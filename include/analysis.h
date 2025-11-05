#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "data_structures.h"
using namespace std;

/**
 *  Runs the core MCMF algorithm, sets up super nodes, and outputs the result.
 */
void run_analysis(vector<Place>& places, ConnectionList& connections);

/**
 *  Provides crop suggestions based on the water flow into the place and soil type.
 */
void suggest_crops(const Place& place, const WaterNetwork& graph);

/**
 *  Calculates the cost and flow for water transfer between a specific surplus and deficit place.
 */
void calculate_specific_transfer_cost(const vector<Place>& places, const WaterNetwork& graph);

/**
 *  Identifies pipes that are operating at maximum capacity.
 */
void identify_bottlenecks(const vector<Place>& places, const WaterNetwork& graph);

#endif // ANALYSIS_H
