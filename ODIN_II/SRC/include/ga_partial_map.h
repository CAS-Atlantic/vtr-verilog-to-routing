#ifndef GA_PARTIAL_MAP_H
#define GA_PARTIAL_MAP_H

#include "odin_types.h"

typedef short ga_type_e;
typedef void (*instantiate_soft_logic_fn_t)(ga_type_e, nnode_t*, short, netlist_t*);
struct connection_t {
    signal_list_t* input;
    signal_list_t* output;
};

struct ga_t {
    nnode_t** node;
    connection_t*** connectivity;
    int size;

    operation_list op;
    instantiate_soft_logic_fn_t instantiate_soft_logic;
    int type_length;
    int initial_type;
};

// Declarations
ga_t* new_ga_t(operation_list op, instantiate_soft_logic_fn_t fn, int type_length, int initial_type);
void add_to_ga_t(ga_t* list, nnode_t* node);
ga_t* free_ga_t(ga_t* list);

void partial_map_ga_item_top(ga_t* list, netlist_t* netlist, short traverse_number);

#endif //GA_PARTIAL_MAP_H
