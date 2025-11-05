#ifndef MCMF_SOLVER_H
#define MCMF_SOLVER_H

#include "data_structures.h"
using namespace std;

/**
 * Finds the lowest cost path using the Bellman-Ford algorithm.
 * PathResult containing the bottleneck flow and the total cost.
 */
PathResult bellman_ford_shortest_path(WaterNetwork& graph, int s, int t, 
                                      vector<int>& parent_v, vector<int>& parent_e);

/**
 *  Calculates the Minimum Cost Maximum Flow (MCMF) using Successive Shortest Path.
 *  max_flow_result Output parameter to store the total flow achieved.
 *  The total minimum cost for the flow pushed.
 */
int min_cost_max_flow(WaterNetwork& graph, int s, int t, int& max_flow_result);

#endif // MCMF_SOLVER_H
