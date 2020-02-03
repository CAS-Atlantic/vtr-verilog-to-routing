
#include "odin_types.h"
#include "odin_globals.h"
#include "netlist_visualizer.h"

#include "netlist_utils.h"
#include "node_creation_library.h"
#include "odin_util.h"

#include "partial_map.h"
#include "multipliers.h"
#include "hard_blocks.h"
#include "math.h"
#include "memories.h"
#include "adders.h"
#include "subtractions.h"
#include "vtr_memory.h"
#include "vtr_util.h"

#include "ga_partial_map.h"
#include "netlist_statistic.h"

struct generation_list_t {
    net_stat_t*** stat;
    ga_type_e** generation;
};

connection_t* new_connection(nnode_t* parent);
connection_t* free_connection(connection_t* to_delete);

generation_list_t* new_generation(ga_t* list, int traverse_mark_number);
generation_list_t* free_generation_list(ga_t* list, generation_list_t* to_delete);
int reinitialize_generation(ga_t* list, generation_list_t* gen, int best_generation, int type_length);
int find_best_generation(ga_t* list, generation_list_t* gen);
void finalize_ga_item_with(ga_t* list, ga_type_e* type_list);

void free_ga_items_in_list(ga_t* list);

void partial_map_ga_item(ga_t* list, generation_list_t* generation, int generation_index, netlist_t* netlist, short traverse_number);
void mutate(ga_type_e* dest, ga_type_e* src, int population_size, int type_length);

/*----------------------------------------------------------------------new_generation
 * (function: partial_map_ga_item_top) 
 *--------------------------------------------------------------------*/
void partial_map_ga_item_top(ga_t* list, netlist_t* netlist, short traverse_number) {
    // if we have items in the list we apply GA mapping
    if (list->size) {
        printf("Partial Mapping %s to target device", name_based_on_op(list->op));
        if (configuration.ga_partial_map) {
            generation_list_t* generation = new_generation(list, traverse_number);

            // run the first partial map
            partial_map_ga_item(list, generation, 0, netlist, traverse_number);

            for (int i = 0; i < configuration.generation_count; i += 1) {
                for (int j = 1; j < configuration.generation_size; j += 1) {
                    partial_map_ga_item(list, generation, j, netlist, traverse_number);
                }

                int best_generation = find_best_generation(list, generation);
                reinitialize_generation(list, generation, best_generation, list->type_length);

                // place the best circuit back
                partial_map_ga_item(list, generation, 0, netlist, traverse_number);

                // force to recompute the whole tree
                traverse_number += 1;
                compute_statistics(netlist, traverse_number);
            }
            finalize_ga_item_with(list, generation->generation[0]);
            free_generation_list(list, generation);

        } else // use default
        {
            for (int i = 0; i < list->size; i += 1) {
                list->instantiate_soft_logic(list->initial_type, list->node[i], -1, netlist);
            }
        }

        free_ga_items_in_list(list);
        printf(" -- Done\n");
    }
}

/*----------------------------------------------------------------------
 * (function: partial_map_adders) This function employ the new type of 
 * adders into the adder list and after that, it instantiates adders and 
 * shrinks them into logics
 *--------------------------------------------------------------------*/
void partial_map_ga_item(ga_t* list, generation_list_t* generation, int generation_index, netlist_t* netlist, short traverse_number) {
    ga_type_e* type_list = generation->generation[generation_index];
    for (int i = 0; i < list->size; i += 1) {
        connection_t* conn = list->connectivity[type_list[i]][i];

        if (conn != NULL) {
            // we have the connections, we can simply remap the I/O
            reattach(conn->input, UPWARD);
            reattach(conn->output, DOWNWARD);
        } else {
            nnode_t* node_duplicate = duplicate_nnode(list->node[i]);
            list->connectivity[type_list[i]][i] = new_connection(node_duplicate);
            conn = list->connectivity[type_list[i]][i];

            // we do the instanciation
            list->instantiate_soft_logic(type_list[i], node_duplicate, -1, netlist);
        }
        if (generation->stat[generation_index][i]) {
            delete_stat(generation->stat[generation_index][i]);
        }
        generation->stat[generation_index][i] = get_stats(conn->input, conn->output, traverse_number);
    }
}

/*----------------------------------------------------------------------
 * (function: partial_map_adders) This function employ the new type of 
 * adders into the adder list and after that, it instantiates adders and 
 * shrinks them into logics
 *--------------------------------------------------------------------*/
void finalize_ga_item_with(ga_t* list, ga_type_e* type_list) {
    for (int i = 0; i < list->size; i += 1) {
        connection_t* conn = list->connectivity[type_list[i]][i];
        list->connectivity[type_list[i]][i] = NULL;

        reattach(conn->input, UPWARD);
        reattach(conn->output, DOWNWARD);
        free_connection(conn);
    }
}

connection_t* new_connection(nnode_t* parent) {
    connection_t* connection = NULL;
    if (parent) {
        connection = (connection_t*)vtr::calloc(1, sizeof(connection_t));
        // get the current stats
        connection->input = init_signal_list();
        if (parent->input_pins
            && parent->num_input_pins) {
            connection->input->count = parent->num_input_pins;
            connection->input->pins = (npin_t**)vtr::calloc(parent->num_input_pins, sizeof(npin_t*));
            for (int i = 0; i < connection->input->count; i += 1) {
                // directly copy the pointer here
                connection->input->pins[i] = parent->input_pins[i];
            }
        }

        connection->output = init_signal_list();
        if (parent->output_pins
            && parent->num_output_pins) {
            connection->output->count = parent->num_output_pins;
            connection->output->pins = (npin_t**)vtr::calloc(parent->num_output_pins, sizeof(npin_t*));
            for (int i = 0; i < connection->output->count; i += 1) {
                // directly copy the pointer here
                connection->output->pins[i] = parent->output_pins[i];
            }
        }
    }

    return connection;
}

connection_t* free_connection(connection_t* to_delete) {
    if (to_delete) {
        free_signal_list(to_delete->input);
        to_delete->input = NULL;
        free_signal_list(to_delete->output);
        to_delete->output = NULL;
        vtr::free(to_delete);
        to_delete = NULL;
    }
    return to_delete;
}

ga_t* new_ga_t(operation_list op, instantiate_soft_logic_fn_t instantiate_soft_logic, int type_length, int initial_type) {
    ga_t* ga_item_list = (ga_t*)vtr::calloc(1, sizeof(ga_t));

    ga_item_list->op = op;
    ga_item_list->node = NULL;
    ga_item_list->size = 0;
    ga_item_list->instantiate_soft_logic = instantiate_soft_logic;
    ga_item_list->type_length = type_length;
    ga_item_list->initial_type = initial_type;
    ga_item_list->connectivity = (connection_t***)vtr::calloc(type_length, sizeof(connection_t**));

    return ga_item_list;
}

void add_to_ga_t(ga_t* list, nnode_t* node) {
    if (node) {
        list->node = (nnode_t**)vtr::realloc(list->node, sizeof(nnode_t*) * (list->size + 1));
        list->node[list->size] = node;

        for (int i = 0; i < list->type_length; i++) {
            list->connectivity[i] = (connection_t**)vtr::realloc(list->connectivity[i], sizeof(connection_t*) * (list->size + 1));
            list->connectivity[i][list->size] = NULL;
        }

        list->size += 1;
    }
}

void free_ga_items_in_list(ga_t* list) {
    if (list) {
        if (list->node) {
            for (int i = 0; i < list->size; i++) {
                free_nnode(list->node[i]);
            }
            vtr::free(list->node);
            list->node = NULL;
        }
        if (list->connectivity) {
            for (int i = 0; i < list->type_length; i++) {
                for (int j = 0; j < list->size; j++) {
                    connection_t* conn = list->connectivity[i][j];
                    list->connectivity[i][j] = NULL;
                    if (conn) {
                        recursive_remove_subtree(conn->input, conn->output);
                        // free leftover pins alloc'd
                        for (int k = 0; k < conn->input->count; k += 1) {
                            free_npin(conn->input->pins[k]);
                        }
                        for (int k = 0; k < conn->output->count; k += 1) {
                            free_npin(conn->output->pins[k]);
                        }
                        free_connection(conn);
                    }
                }
                vtr::free(list->connectivity[i]);
                list->connectivity[i] = NULL;
            }

            vtr::free(list->connectivity);
            list->connectivity = NULL;
        }
        list->size = 0;
    }
}

ga_t* free_ga_t(ga_t* list) {
    if (list) {
        free_ga_items_in_list(list);
        vtr::free(list);
        list = NULL;
    }
    return list;
}

/*--------------------------------------------------------------------- 
 * This function creates a new generation. It changes the values of the 
 * genes by doing mutation. In each generation, except of the first one,
 * the parent of the next generation will be the fittest chromosome in 
 * the previous generation.
 *--------------------------------------------------------------------*/
generation_list_t* new_generation(ga_t* list, int traverse_number) {
    // Initilization of new generation
    generation_list_t* new_generation = (generation_list_t*)vtr::calloc(1, sizeof(generation_list_t));

    new_generation->generation = (ga_type_e**)vtr::calloc(configuration.generation_size, sizeof(ga_type_e*));
    new_generation->stat = (net_stat_t***)vtr::calloc(configuration.generation_size, sizeof(net_stat_t**));

    for (int i = 0; i < configuration.generation_size; i += 1) {
        new_generation->generation[i] = (ga_type_e*)vtr::calloc(list->size, sizeof(ga_type_e));
        new_generation->stat[i] = (net_stat_t**)vtr::calloc(list->size, sizeof(net_stat_t*));
    }

    // initialize the parent generation
    for (int i = 0; i < list->size; i += 1) {
        new_generation->stat[0][i] = get_stats(list->node[i], traverse_number);
        new_generation->generation[0][i] = list->initial_type;
    }
    return new_generation;
}

generation_list_t* free_generation_list(ga_t* list, generation_list_t* to_delete) {
    if (to_delete) {
        if (to_delete->generation) {
            for (int i = 0; i < configuration.generation_size; i += 1) {
                vtr::free(to_delete->generation[i]);

                if (to_delete->stat[i]) {
                    for (int j = 0; j < list->size; j += 1) {
                        delete_stat(to_delete->stat[i][j]);
                    }
                }
                vtr::free(to_delete->stat[i]);
            }
            vtr::free(to_delete->generation);
            vtr::free(to_delete->stat);
        }
        vtr::free(to_delete);
        to_delete = NULL;
    }
    return to_delete;
}

int reinitialize_generation(ga_t* list, generation_list_t* gen, int best_generation, int type_length) {
    int parent_generation = 0;
    // swap the best generation to index 0
    ga_type_e* fittest = gen->generation[best_generation];
    gen->generation[best_generation] = gen->generation[parent_generation];
    gen->generation[parent_generation] = fittest;

    // fittest is [0] so we skip
    for (int i = parent_generation + 1; i < configuration.generation_size; i += 1) {
        // The children of each generation will be a mutation from the fittest
        mutate(gen->generation[i], fittest, list->size, type_length);
    }
    return parent_generation;
}

int find_best_generation(ga_t* /*list*/, generation_list_t* /*gen*/) {
    // for now we randomly select
    int i = rand() % configuration.generation_size;
    return i;
}

/*--------------------------------------------------------------------- 
 * This function changes the value of the genes in the given chromosome.
 * Based on the specified mutation rate, it finds the position of genes 
 * which should be mutated randomly, and after that, it changes their 
 * values through rand function.
 *--------------------------------------------------------------------*/
void mutate(ga_type_e* dest, ga_type_e* src, int population_size, int type_length) {
    // Find a point and flip the chromosome from there
    // int cross_point = rand()%num_of_adders;
    // Find points based on the mutation rate and change their values randomly
    int num_of_genes_will_be_changed = configuration.mutation_rate * population_size;

    for (int i = 0; i < population_size; i += 1) {
        dest[i] = src[i];
    }

    while (num_of_genes_will_be_changed != 0) {
        int i = rand() % population_size;
        dest[i] = (ga_type_e)(rand() % type_length);

        num_of_genes_will_be_changed -= 1;
    }
}