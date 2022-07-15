#include <queue>
#include <random>
#include <algorithm>

#include "vtr_memory.h"
#include "vtr_time.h"

#include "vpr_types.h"
#include "vpr_error.h"

#include "globals.h"
#include "rr_graph_util.h"

int seg_index_of_cblock(t_rr_type from_rr_type, int to_node) {
    /* Returns the segment number (distance along the channel) of the connection *
     * box from from_rr_type (CHANX or CHANY) to to_node (IPIN).                 */

    auto& device_ctx = g_vpr_ctx.device();
    const auto& rr_graph = device_ctx.rr_graph;

    if (from_rr_type == CHANX)
        return (rr_graph.node_xlow(RRNodeId(to_node)));
    else
        /* CHANY */
        return (rr_graph.node_ylow(RRNodeId(to_node)));
}

int seg_index_of_sblock(int from_node, int to_node) {
    /* Returns the segment number (distance along the channel) of the switch box *
     * box from from_node (CHANX or CHANY) to to_node (CHANX or CHANY).  The     *
     * switch box on the left side of a CHANX segment at (i,j) has seg_index =   *
     * i-1, while the switch box on the right side of that segment has seg_index *
     * = i.  CHANY stuff works similarly.  Hence the range of values returned is *
     * 0 to device_ctx.grid.width()-1 (if from_node is a CHANX) or 0 to device_ctx.grid.height()-1 (if from_node is a CHANY).   */

    t_rr_type from_rr_type, to_rr_type;

    auto& device_ctx = g_vpr_ctx.device();
    const auto& rr_graph = device_ctx.rr_graph;

    from_rr_type = rr_graph.node_type(RRNodeId(from_node));
    to_rr_type = rr_graph.node_type(RRNodeId(to_node));

    if (from_rr_type == CHANX) {
        if (to_rr_type == CHANY) {
            return (rr_graph.node_xlow(RRNodeId(to_node)));
        } else if (to_rr_type == CHANX) {
            if (rr_graph.node_xlow(RRNodeId(to_node)) > rr_graph.node_xlow(RRNodeId(from_node))) { /* Going right */
                return (rr_graph.node_xhigh(RRNodeId(from_node)));
            } else { /* Going left */
                return (rr_graph.node_xhigh(RRNodeId(to_node)));
            }
        } else {
            VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                            "in seg_index_of_sblock: to_node %d is of type %d.\n",
                            to_node, to_rr_type);
            return OPEN; //Should not reach here once thrown
        }
    }
    /* End from_rr_type is CHANX */
    else if (from_rr_type == CHANY) {
        if (to_rr_type == CHANX) {
            return (rr_graph.node_ylow(RRNodeId(to_node)));
        } else if (to_rr_type == CHANY) {
            if (rr_graph.node_ylow(RRNodeId(to_node)) > rr_graph.node_ylow(RRNodeId(from_node))) { /* Going up */
                return (rr_graph.node_yhigh(RRNodeId(from_node)));
            } else { /* Going down */
                return (rr_graph.node_yhigh(RRNodeId(to_node)));
            }
        } else {
            VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                            "in seg_index_of_sblock: to_node %d is of type %d.\n",
                            to_node, to_rr_type);
            return OPEN; //Should not reach here once thrown
        }
    }
    /* End from_rr_type is CHANY */
    else {
        VPR_FATAL_ERROR(VPR_ERROR_ROUTE,
                        "in seg_index_of_sblock: from_node %d is of type %d.\n",
                        from_node, from_rr_type);
        return OPEN; //Should not reach here once thrown
    }
}

bool is_node_on_tile(t_rr_type node_type,
                     int root_x,
                     int root_y,
                     int node_ptc) {
    if(node_type == CHANX || node_type == CHANY) {
        return true;
    } else {
        VTR_ASSERT(node_type == IPIN || node_type == SINK || node_type == OPIN || node_type == SOURCE);
        auto& device_ctx = g_vpr_ctx.device();
        t_physical_tile_type_ptr tile_type = device_ctx.grid[root_x][root_y].type;
        if (node_type == IPIN || node_type == OPIN) {
            return is_pin_on_tile(tile_type, node_ptc);
        } else {
            VTR_ASSERT(node_type == SINK || node_type == SOURCE);
            return is_class_on_tile(tile_type, node_ptc);
        }
    }

}

int get_rr_node_max_ptc (const RRGraphView& rr_graph_view,
                        RRNodeId node_id,
                        bool is_flat) {
    auto node_type = rr_graph_view.node_type(node_id);

    VTR_ASSERT(node_type == IPIN || node_type == OPIN ||
               node_type == SINK || node_type == SOURCE);

    const DeviceContext& device_ctx = g_vpr_ctx.device();
    auto physical_type=
        device_ctx.grid[rr_graph_view.node_xlow(node_id)][rr_graph_view.node_ylow(node_id)].type;
    
    if(node_type == SINK || node_type == SOURCE) {
        return get_tile_class_max_ptc(physical_type, is_flat);
    } else {
        return get_tile_ipin_opin_max_ptc(physical_type, is_flat);
    }
}

RRNodeId get_pin_rr_node_id(const RRSpatialLookup& rr_spatial_lookup,
                                   t_physical_tile_type_ptr physical_tile,
                                   const int i,
                                   const int j,
                                   int pin_physical_num) {
    RRNodeId node_id = RRNodeId::INVALID();

    e_pin_type pin_type = get_pin_type_from_pin_physical_num(physical_tile, pin_physical_num);
    VTR_ASSERT(pin_type == DRIVER || pin_type == RECEIVER);
    t_rr_type node_type = (pin_type == e_pin_type::DRIVER) ? t_rr_type::OPIN : t_rr_type::IPIN;
    if(is_pin_on_tile(physical_tile, pin_physical_num)){
        for (int width = 0; width < physical_tile->width; ++width) {
            for (int height = 0; height < physical_tile->height; ++height) {
                for (e_side side : SIDES) {
                    if (physical_tile->pinloc[width][height][side][pin_physical_num]) {
                        node_id = rr_spatial_lookup.find_node(i+width, j+height, node_type, pin_physical_num, side);
                        if(node_id != RRNodeId::INVALID())
                            return node_id;
                    }
                }
            }

        }
    } else {
        node_id = rr_spatial_lookup.find_node(i, j, node_type, pin_physical_num, e_side::TOP);
        return node_id;
    }

    return RRNodeId::INVALID();
}

RRNodeId get_class_rr_node_id(const RRSpatialLookup& rr_spatial_lookup,
                              t_physical_tile_type_ptr physical_tile,
                              const int i,
                              const int j,
                              int class_physical_num) {


    auto class_type = get_class_type_from_class_physical_num(physical_tile, class_physical_num);
    VTR_ASSERT(class_type == DRIVER || class_type == RECEIVER);
    t_rr_type node_type = (class_type == e_pin_type::DRIVER) ? t_rr_type::SOURCE : t_rr_type::SINK;
    return rr_spatial_lookup.find_node(i, j, node_type, class_physical_num);
}

vtr::vector<RRNodeId, std::vector<RREdgeId>> get_fan_in_list() {
    auto& rr_graph = g_vpr_ctx.device().rr_graph;

    vtr::vector<RRNodeId, std::vector<RREdgeId>> node_fan_in_list;

    node_fan_in_list.resize(rr_graph.num_nodes(), std::vector<RREdgeId>(0));
    node_fan_in_list.shrink_to_fit();

    //Walk the graph and increment fanin on all dwnstream nodes
    rr_graph.rr_nodes().for_each_edge(
        [&](RREdgeId edge, __attribute__((unused)) RRNodeId src, RRNodeId sink) {
            node_fan_in_list[sink].push_back(edge);
        });

    return node_fan_in_list;
}
