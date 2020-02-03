#ifndef REGISTERED_GA_ITEMS_H
#define REGISTERED_GA_ITEMS_H

#include "odin_types.h"

void register_ga();
void deregister_ga();

void GA_partial_map_top(netlist_t* netlist);
void add_to_ga(operation_list op, nnode_t* node);

#endif // REGISTERED_GA_ITEMS_H