#include "graph_ops.h"

/**
 * Adds both the forward and reverse edges for a road, initializing flow to 0.
 * The reverse edge has capacity 0 and negative cost, used in the residual graph.
 */
void add_edge(WaterNetwork& graph, int u, int v, int cap, int cost) {
    // 1. Forward edge (u -> v)
    // Stores the index of the soon-to-be-added reverse edge.
    Edge forward = {v, cap, 0, cost, (int)graph[v].size()}; 
    graph[u].push_back(forward);

    // 2. Backward (Residual) edge (v -> u)
    // Stores the index of the forward edge just added.
    Edge backward = {u, 0, 0, -cost, (int)graph[u].size() - 1}; 
    graph[v].push_back(backward);
}
