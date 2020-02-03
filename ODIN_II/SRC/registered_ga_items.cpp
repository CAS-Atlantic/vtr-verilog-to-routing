#include "registered_ga_items.h"
#include "ga_partial_map.h"
#include "partial_map.h"
#include "odin_error.h"
#include "odin_util.h"

// global with a ga for each op
ga_t* ga_op[operation_list_END] = {0};

// fn definition
void new_ga(operation_list op, instantiate_soft_logic_fn_t fn, int type_length, int type_init);
void instantiate_add_w_carry(ga_type_e type, nnode_t* node, short mark, netlist_t* netlist);

void register_ga() {
    /* register the build functions here */
    new_ga(ADD, instantiate_add_w_carry, adder_type_END, RCA);
}

void deregister_ga() {
    for (int i = 0; i < operation_list_END; i++) {
        if (ga_op[i]) {
            free_ga_t(ga_op[i]);
        }
    }
}

void GA_partial_map_top(netlist_t* netlist) {
    /* ordering of optimization is crutial */

    // we do adder last
    partial_map_ga_item_top(ga_op[ADD], netlist, PARTIAL_MAP_TRAVERSE_VALUE_GA_ADDERS);
}

/* wrappers to cast back to type */
void instantiate_add_w_carry(ga_type_e type, nnode_t* node, short mark, netlist_t* netlist) {
    instantiate_add_w_carry((adder_type_e)type, node, mark, netlist);
}

/* wrapper to hide the ga calls */
void new_ga(operation_list op, instantiate_soft_logic_fn_t fn, int type_length, int type_init) {
    if (NULL != ga_op[op]) {
        error_message(NETLIST_ERROR, -1, -1,
                      "%s already has registered GA type. unable to proceed",
                      name_based_on_op(op));
    }
    ga_op[op] = new_ga_t(op, fn, type_length, type_init);
}

void add_to_ga(operation_list op, nnode_t* node) {
    if (NULL == ga_op[op]) {
        error_message(NETLIST_ERROR, -1, -1,
                      "%s does not have a registered GA type. unable to proceed",
                      name_based_on_op(op));
    }

    add_to_ga_t(ga_op[op], node);
}