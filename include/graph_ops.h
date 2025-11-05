#ifndef GRAPH_OPS_H
#define GRAPH_OPS_H

#include "data_structures.h"

/**
 *  Adds a forward edge (u -> v) and its reverse edge (v -> u) to the graph.
 *  graph The network adjacency list.
 *  u Starting place ID.
 *  v Ending place ID.
 *  cap Maximum capacity of the pipe.
 *  cost Base cost per unit of flow.
 */
void add_edge(WaterNetwork& graph, int u, int v, int cap, int cost);

#endif // GRAPH_OPS_H
