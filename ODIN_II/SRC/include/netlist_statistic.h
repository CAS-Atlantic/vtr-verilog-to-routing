#ifndef NETLIST_STATISTIC_HPP
#define NETLIST_STATISTIC_HPP

#include "netlist_utils.h"

void compute_statistics(netlist_t* netlist, int traverse_mark_number);

net_stat_t* get_stats(signal_list_t* input_signals, signal_list_t* output_signals, int traverse_mark_number);
net_stat_t* get_stats(nnode_t* node, int traverse_mark_number);
net_stat_t* get_stats(nnet_t* net, int traverse_mark_number);
net_stat_t* delete_stat(net_stat_t* stat);

#endif // NETLIST_STATISTIC_HPP
