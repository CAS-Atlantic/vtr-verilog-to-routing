#include <algorithm>

#include "odin_types.h"
#include "netlist_statistic.h"
#include "vtr_memory.h"

static metric_t* new_metric(void);

static void init(metric_t* m);
static void copy(metric_t* dest, metric_t* src);
static void compute_stat(metric_t* destination, metric_t** sources, int source_count);

static metric_t* get_upward_net_stat(nnet_t* net, int traverse_mark_number);
static metric_t* get_downward_net_stat(nnet_t* net, int traverse_mark_number);
static void store_upward_node_stat(metric_t* destination, nnode_t* node, int traverse_mark_number);
static void store_downward_node_stat(metric_t* destination, nnode_t* node, int traverse_mark_number);
static metric_t* get_downward_signal_stat(metric_t* destination, signal_list_t* signals, int traverse_mark_number);
static metric_t* get_upward_signal_stat(metric_t* destination, signal_list_t* signals, int traverse_mark_number);

metric_t* new_metric(void) {
    metric_t* m = (metric_t*)vtr::malloc(sizeof(metric_t));
    init(m);
    return m;
}

static void init(metric_t* m) {
    m->min_depth = 0;
    m->max_depth = 0;
    m->avg_depth = 0;
    m->avg_width = 0;
}

static void copy(metric_t* dest, metric_t* src) {
    if (dest) {
        init(dest);
        if (src) {
            dest->min_depth = src->min_depth;
            dest->max_depth = src->max_depth;
            dest->avg_depth = src->avg_depth;
            dest->avg_width = src->avg_width;
        }
    }
}

static void compute_stat(metric_t* destination, metric_t** sources, int source_count) {
    init(destination);
    int actual_count = 0;
    // compute stats from parent
    for (int i = 0; i < source_count; i += 1) {
        if (sources && sources[i]) {
            actual_count += 1;

            if (destination->min_depth == 0) {
                destination->min_depth = sources[i]->min_depth;
                destination->max_depth = sources[i]->max_depth;
                destination->avg_depth = sources[i]->avg_depth;
                destination->avg_width = sources[i]->avg_width;
            } else {
                destination->min_depth = std::min(sources[i]->min_depth, destination->min_depth);
                destination->max_depth = std::max(sources[i]->max_depth, destination->max_depth);
                destination->avg_depth += sources[i]->avg_depth;
                destination->avg_width += sources[i]->avg_width;
            }
        }
    }

    if (actual_count) {
        destination->avg_depth /= actual_count;
        destination->avg_width /= actual_count;
    }

    destination->min_depth += 1;
    destination->max_depth += 1;
    destination->avg_depth += 1;
    destination->avg_width += source_count;
}

static metric_t* get_upward_net_stat(nnet_t* net, int traverse_mark_number) {
    metric_t* destination = NULL;
    if (net) {
        destination = &(net->stat.upward);
        if (net->traverse_visited != traverse_mark_number) {
            net->traverse_visited = traverse_mark_number;
            if (net->driver_pin) {
                nnode_t* node = net->driver_pin->node;
                store_upward_node_stat(destination, node, traverse_mark_number);
            } else {
                // base case
                init(destination);
            }
        }
    }
    return destination;
}

static metric_t* get_downward_net_stat(nnet_t* net, int traverse_mark_number) {
    metric_t* destination = NULL;
    if (net) {
        destination = &(net->stat.downward);
        if (net->traverse_visited != traverse_mark_number) {
            net->traverse_visited = traverse_mark_number;
            metric_t** child_stat = (metric_t**)vtr::malloc(net->num_fanout_pins * sizeof(metric_t*));
            for (int i = 0; i < net->num_fanout_pins; i++) {
                child_stat[i] = NULL;
                if (net->fanout_pins[i]) {
                    child_stat[i] = new_metric();
                    nnode_t* node = net->fanout_pins[i]->node;
                    store_downward_node_stat(child_stat[i], node, traverse_mark_number);
                }
            }
            compute_stat(destination, child_stat, net->num_fanout_pins);

            for (int i = 0; i < net->num_fanout_pins; i++) {
                vtr::free(child_stat[i]);
            }
            vtr::free(child_stat);
        }
    }
    return destination;
}

static void store_upward_node_stat(metric_t* destination, nnode_t* node, int traverse_mark_number) {
    if (node) {
        metric_t** parent_stat = (metric_t**)vtr::malloc(node->num_input_pins * sizeof(metric_t*));
        for (int i = 0; i < node->num_input_pins; i++) {
            parent_stat[i] = NULL;
            if (node->input_pins[i]) {
                nnet_t* net = node->input_pins[i]->net;
                parent_stat[i] = get_upward_net_stat(net, traverse_mark_number);
            }
        }
        compute_stat(destination, parent_stat, node->num_input_pins);
        vtr::free(parent_stat);
    }
}

static void store_downward_node_stat(metric_t* destination, nnode_t* node, int traverse_mark_number) {
    if (node) {
        metric_t** child_stat = (metric_t**)vtr::malloc(node->num_output_pins * sizeof(metric_t*));
        for (int i = 0; i < node->num_output_pins; i++) {
            child_stat[i] = NULL;
            if (node->output_pins[i]) {
                nnet_t* net = node->output_pins[i]->net;
                child_stat[i] = get_downward_net_stat(net, traverse_mark_number);
            }
        }
        compute_stat(destination, child_stat, node->num_output_pins);
        vtr::free(child_stat);
    }
}

static metric_t* get_downward_signal_stat(metric_t* destination, signal_list_t* signals, int traverse_mark_number) {
    if (signals) {
        metric_t** child_stat = (metric_t**)vtr::malloc(signals->count * sizeof(metric_t*));
        for (int i = 0; i < signals->count; i++) {
            child_stat[i] = NULL;
            if (signals->pins[i]) {
                // node -> net
                if (signals->pins[i]->type == OUTPUT) {
                    child_stat[i] = get_downward_net_stat(signals->pins[i]->net, traverse_mark_number);
                }
                // net -> node
                else {
                    child_stat[i] = new_metric();
                    store_downward_node_stat(child_stat[i], signals->pins[i]->node, traverse_mark_number);
                }
            }
        }
        compute_stat(destination, child_stat, signals->count);

        for (int i = 0; i < signals->count; i++) {
            if (signals->pins[i]) {
                if (signals->pins[i]->type == INPUT) {
                    vtr::free(child_stat[i]);
                }
            }
        }

        vtr::free(child_stat);
    }
    return destination;
}

static metric_t* get_upward_signal_stat(metric_t* destination, signal_list_t* signals, int traverse_mark_number) {
    if (signals) {
        metric_t** child_stat = (metric_t**)vtr::malloc(signals->count * sizeof(metric_t*));
        for (int i = 0; i < signals->count; i++) {
            child_stat[i] = NULL;
            if (signals->pins[i]) {
                // net -> node
                if (signals->pins[i]->type == INPUT) {
                    child_stat[i] = get_upward_net_stat(signals->pins[i]->net, traverse_mark_number);
                }
                // node -> net
                else {
                    child_stat[i] = new_metric();
                    store_upward_node_stat(child_stat[i], signals->pins[i]->node, traverse_mark_number);
                }
            }
        }
        compute_stat(destination, child_stat, signals->count);

        for (int i = 0; i < signals->count; i++) {
            if (signals->pins[i]) {
                if (signals->pins[i]->type == OUTPUT) {
                    vtr::free(child_stat[i]);
                }
            }
        }

        vtr::free(child_stat);
    }
    return destination;
}

net_stat_t* get_stats(signal_list_t* input_signals, signal_list_t* output_signals, int traverse_mark_number) {
    net_stat_t* stat = (net_stat_t*)vtr::malloc(sizeof(net_stat_t));
    // we recompute bellow the input since this is were connection can change
    get_downward_signal_stat(&stat->downward, input_signals, traverse_mark_number);
    // we recompute abov the output since this is were connection can change
    get_upward_signal_stat(&stat->upward, output_signals, traverse_mark_number);
    return stat;
}

net_stat_t* get_stats(nnode_t* node, int traverse_mark_number) {
    net_stat_t* stat = (net_stat_t*)vtr::malloc(sizeof(net_stat_t));
    store_downward_node_stat(&stat->downward, node, traverse_mark_number);
    store_upward_node_stat(&stat->upward, node, traverse_mark_number);
    return stat;
}

net_stat_t* get_stats(nnet_t* net, int traverse_mark_number) {
    net_stat_t* stat = (net_stat_t*)vtr::malloc(sizeof(net_stat_t));
    copy(&stat->downward, get_downward_net_stat(net, traverse_mark_number));
    copy(&stat->upward, get_upward_net_stat(net, traverse_mark_number));
    return stat;
}

net_stat_t* delete_stat(net_stat_t* stat) {
    if (stat) {
        vtr::free(stat);
        stat = NULL;
    }
    return stat;
}

/*---------------------------------------------------------------------------------------------
 * function: dfs_to_cp() it starts from head and end of the netlist to calculate critical path
 *-------------------------------------------------------------------------------------------*/
void compute_statistics(netlist_t* netlist, int traverse_mark_number) {
    if (!netlist) {
        return;
    }

    metric_t temp;
    init(&temp);
    /* start with the primary input list */
    for (int i = 0; i < netlist->num_top_input_nodes; i++) {
        if (netlist->top_input_nodes[i] != NULL) {
            store_downward_node_stat(&temp, netlist->top_input_nodes[i], traverse_mark_number);
        }
    }

    /* now traverse the ground and vcc pins  */
    store_downward_node_stat(&temp, netlist->vcc_node, traverse_mark_number);
    store_downward_node_stat(&temp, netlist->pad_node, traverse_mark_number);
    store_downward_node_stat(&temp, netlist->gnd_node, traverse_mark_number);

    for (int i = 0; i < netlist->num_top_output_nodes; i++) {
        if (netlist->top_output_nodes[i] != NULL) {
            store_upward_node_stat(&temp, netlist->top_output_nodes[i], traverse_mark_number);
        }
    }
}