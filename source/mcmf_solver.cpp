#include "mcmf_solver.h"
#include <algorithm>
#include <iostream>

using namespace std;

/**
 * Finds the lowest cost path from source 's' to sink 't' using Bellman-Ford.
 * Since costs can be negative in the residual graph, Bellman-Ford is used.
 */
PathResult bellman_ford_shortest_path(WaterNetwork& graph, int s, int t, 
                                      vector<int>& parent_v, vector<int>& parent_e) {
    int N = graph.size();
    vector<int> dist(N, INT_MAX); // Distance (cost) from source
    vector<int> path_flow(N, 0);  // Max flow capacity to this node

    dist[s] = 0;
    path_flow[s] = INT_MAX;
    
    fill(parent_v.begin(), parent_v.end(), -1);
    fill(parent_e.begin(), parent_e.end(), -1);

    // V-1 iterations of relaxation
    for (int i = 1; i < N; ++i) {
        bool updated = false;
        for (int u = 0; u < N; ++u) {
            for (size_t edge_idx = 0; edge_idx < graph[u].size(); ++edge_idx) {
                const auto& edge = graph[u][edge_idx];
                int v = edge.to_place;
                int residual_capacity = edge.capacity - edge.flow;

                if (residual_capacity > 0 && dist[u] != INT_MAX) {
                    int new_dist = dist[u] + edge.cost;
                    
                    if (dist[v] == INT_MAX || dist[v] > new_dist) {
                        dist[v] = new_dist;
                        parent_v[v] = u;
                        parent_e[v] = (int)edge_idx;
                        path_flow[v] = min(path_flow[u], residual_capacity);
                        updated = true;
                    }
                }
            }
        }
        if (!updated) break; 
    }

    if (dist[t] == INT_MAX) {
        return {0, 0}; // No path found
    }
    
    return {path_flow[t], dist[t]};
}

/**
 * Calculates the Minimum Cost Maximum Flow (MCMF) using Successive Shortest Path.
 */
int min_cost_max_flow(WaterNetwork& graph, int s, int t, int& max_flow_result) {
    int total_cost = 0;
    max_flow_result = 0;
    int N = graph.size();
    
    vector<int> parent_v(N); 
    vector<int> parent_e(N); 

    PathResult result;

    // Loop until no more flow can be pushed
    while ((result = bellman_ford_shortest_path(graph, s, t, parent_v, parent_e)).flow > 0) {
        int path_flow = result.flow;
        int path_cost = result.cost;

        max_flow_result += path_flow;
        total_cost += path_flow * path_cost;
        
        // Augment flow along the path
        int v = t;
        while (v != s) {
            int u = parent_v[v];
            int edge_idx = parent_e[v];
            
            // 1. Forward edge (u -> v): Increase flow
            auto& forward_edge = graph[u][edge_idx];
            forward_edge.flow += path_flow;
            
            // 2. Reverse edge (v -> u): Decrease flow (which increases residual capacity)
            int reverse_idx = forward_edge.reverse_edge;
            graph[v][reverse_idx].flow -= path_flow; 
            
            v = u;
        }
    }

    return total_cost;
}
