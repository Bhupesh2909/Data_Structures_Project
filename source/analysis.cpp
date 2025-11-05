#include "analysis.h"
#include "graph_ops.h"
#include "mcmf_solver.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

// --- Helper for DFS traversal (used in calculate_specific_transfer_cost) ---
bool find_flow_path(const WaterNetwork& graph, int u, int target_v, 
                    int& current_flow, int& current_cost, vector<bool>& visited) {
    if (u == target_v) return true;

    visited[u] = true;

    for (const auto& edge : graph[u]) {
        // Only follow edges that have positive flow and are forward edges (cost >= 0)
        if (edge.flow > 0 && edge.cost >= 0 && !visited[edge.to_place]) {
            
            // Recursively find the path
            if (find_flow_path(graph, edge.to_place, target_v, current_flow, current_cost, visited)) {
                // Path found: update flow and cost along the way back
                current_flow = min(current_flow, edge.flow);
                current_cost += edge.flow * edge.cost;
                return true;
            }
        }
    }
    visited[u] = false; // Backtrack
    return false;
}

// --- Query Implementations ---

/**
 *  Calculates the cost and flow for water transfer between a specific surplus and deficit place.
 *  This is complex in MCMF as flow can split and merge, but this finds ONE path's cost/flow.
 */
void calculate_specific_transfer_cost(const vector<Place>& places, const WaterNetwork& graph) {
    int surplus_id, deficit_id;
    cout << "Enter Source Place ID (Surplus): ";
    cin >> surplus_id;
    cout << "Enter Target Place ID (Deficit): ";
    cin >> deficit_id;

    // FIX: Cast places.size() to int to resolve the signed/unsigned warning (Line 48)
    // This allows us to keep the input IDs as 'int' to safely check for negative inputs.
    if (surplus_id < 0 || surplus_id >= (int)places.size() || deficit_id < 0 || deficit_id >= (int)places.size()) {
        cout << "Invalid Place ID." << endl;
        return;
    }

    if (places[surplus_id].deficit_or_surplus <= 0) {
        cout << "Place " << places[surplus_id].name << " is not a surplus source." << endl;
        return;
    }
     if (places[deficit_id].deficit_or_surplus >= 0) {
        cout << "Place " << places[deficit_id].name << " is not a deficit sink." << endl;
        return;
    }

    // Try to find a single path with flow from source to sink
    int total_flow = INT_MAX; 
    int total_cost = 0;
    vector<bool> visited(graph.size(), false);

    // This is a simplified query; a full flow tracing requires more complex graph analysis
    if (find_flow_path(graph, surplus_id, deficit_id, total_flow, total_cost, visited)) {
        cout << "\n--- Specific Transfer Path Analysis ---\n";
        cout << "From: " << places[surplus_id].name << " -> To: " << places[deficit_id].name << endl;
        cout << "  Flow Found: **" << total_flow << " KL** (Represents bottleneck flow on one path)" << endl;
        cout << "  Cost Along Path: **$" << total_cost << "**" << endl;
    } else {
        cout <<" No direct path with positive flow exists between " 
             << places[surplus_id].name << " and " << places[deficit_id].name << " in the final plan." << endl;
    }
}

/**
 *  Identifies pipes that are operating at maximum capacity.
 */
void identify_bottlenecks(const vector<Place>& places, const WaterNetwork& graph) {
    cout << "\n--- NETWORK BOTTLENECK REPORT ---\n";
    cout << "Edges running at Maximum Capacity:\n";
    bool found = false;
    // FIX: Changed 'int u' to 'size_t u' to avoid signed/unsigned comparison warning (Line 86)
    for (size_t u = 0; u < places.size(); ++u) {
        for (const auto& edge : graph[u]) {
            // Check forward edges (cost >= 0) that have full capacity utilization
            if (edge.cost >= 0 && edge.capacity > 0 && edge.flow == edge.capacity) {
                cout << "  [Pipe " << places[u].name << " (ID " << u << ") -> " 
                     << places[edge.to_place].name << " (ID " << edge.to_place << ")]: "
                     << edge.flow << " / " << edge.capacity << " KL/hr" << endl;
                found = true;
            }
        }
    }
    if (!found) {
        cout << "  No capacity bottlenecks found. The network is under-utilized." << endl;
    }
    cout << "--------------------------------\n";
}


/**
 * Provides crop suggestions based on the water flow into the place and soil type.
 */
void suggest_crops(const Place& place, const WaterNetwork& graph) {
    if (place.soil_type == "None" || place.deficit_or_surplus >= 0) {
        return; // Skip non-farm areas
    }

    // 1. Calculate Water Availability
    int actual_water_received = 0;
    // FIX: Changed 'int u' to 'size_t u' to avoid signed/unsigned comparison warning (Line 114)
    for (size_t u = 0; u < graph.size(); ++u) {
        for (const auto& edge : graph[u]) {
            if (edge.to_place == place.id && edge.flow > 0 && edge.cost >= 0) {
                actual_water_received += edge.flow; 
            }
        }
    }
    
    int required = abs(place.deficit_or_surplus);
    int water_level_score = 0; // 1 (Low) to 5 (High)

    // Determine water score based on fulfillment percentage
    float fulfillment_pct = (required > 0) ? (float)actual_water_received / required : 1.0f;
    
    if (fulfillment_pct >= 0.9) water_level_score = 5;
    else if (fulfillment_pct >= 0.6) water_level_score = 4;
    else if (fulfillment_pct >= 0.4) water_level_score = 3;
    else if (fulfillment_pct >= 0.2) water_level_score = 2;
    else water_level_score = 1;

    cout << "\n--- Suggestions for " << place.name << " ---" << endl;
    cout << "   -> Soil Type: **" << place.soil_type << "**" << endl;
    cout << "   -> Water Status: " << actual_water_received << " KL Received / " 
         << required << " KL Required (" << (fulfillment_pct * 100) << "%)" << endl;
    cout << "   -> Recommendations (Water Score: " << water_level_score << "):" << endl;

    bool suggested = false;
    // Assuming CROP_DATA is defined elsewhere, no change needed here.
    // If CROP_DATA is a vector, this loop should also ideally use size_t, 
    // but since it uses a range-based for loop, it's already safe.
    for (const auto& crop : CROP_DATA) {
        bool sufficient_water = (water_level_score >= crop.water_requirement);
        bool ideal_soil = (place.soil_type == crop.ideal_soil);
        
        if (sufficient_water && ideal_soil) {
            cout << "      - **" << crop.name << "** (HIGH MATCH: Ideal Soil & Water)" << endl;
            suggested = true;
        } else if (sufficient_water) {
            cout << "      - " << crop.name << " (GOOD: Sufficient Water, Soil Mismatch)" << endl;
            suggested = true;
        }
    }
    if (!suggested) {
         cout << "      - *No crops found that match current water level and soil type.*" << endl;
    }
    cout << "------------------------------------------" << endl;
}


/**
 *  Runs the core MCMF algorithm, sets up super nodes, and outputs the result.
 */
void run_analysis(vector<Place>& places, ConnectionList& connections) {
    if (places.empty()) {
        cout << "Cannot run analysis. Please load data first." << endl;
        return;
    }

    // 1. Determine Network Size and Super Nodes
    const size_t NUM_PLACES = places.size();
    const int SUPER_SOURCE = NUM_PLACES;
    const int SUPER_SINK = NUM_PLACES + 1;
    const int TOTAL_NODES = NUM_PLACES + 2;

    WaterNetwork graph(TOTAL_NODES);

    // 2. Add Road/Pipe Connections
    for (const auto& conn : connections) {
        add_edge(graph, get<0>(conn), get<1>(conn), get<2>(conn), get<3>(conn));
    }

    // 3. Add Super Source / Super Sink Connections (Priority Integration)
    int total_required = 0;
    int total_available = 0;

    for (const auto& p : places) {
        if (p.deficit_or_surplus > 0) { // Surplus node: Connect to Super Source
            total_available += p.deficit_or_surplus;
            add_edge(graph, SUPER_SOURCE, p.id, p.deficit_or_surplus, 0);
        } else if (p.deficit_or_surplus < 0) { // Deficit node: Connect to Super Sink
            total_required += abs(p.deficit_or_surplus);
            int required_amount = abs(p.deficit_or_surplus);
            
            // Priority Cost: Lower priority (5) means higher cost/penalty
            int priority_cost = (p.priority_level - 1) * PRIORITY_PENALTY; 
            
            add_edge(graph, p.id, SUPER_SINK, required_amount, priority_cost);
        }
    }

    cout << "\n--- WATER DISTRIBUTION ANALYSIS (MCMF) ---" << endl;
    cout << "Total Required: " << total_required << " KL | Total Available: " << total_available << " KL" << endl;

    // 4. Run MCMF
    int total_flow_achieved = 0;
    int min_total_cost = min_cost_max_flow(graph, SUPER_SOURCE, SUPER_SINK, total_flow_achieved);

    // 5. Output Results
    cout << "--------------------------------------------------------" << endl;
    cout << "Max Flow Achieved (Water Distributed): **" << total_flow_achieved << " KL**" << endl;
    cout << "Minimum Distribution Cost: **$" << min_total_cost << "**" << endl;
    cout < "Cost includes priority penalties.)" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    // --- Post-Analysis Queries ---

    int query_choice;
    do {
        cout << "\n--- POST-ANALYSIS QUERIES ---\n";
        cout << "1. Specific Transfer Cost/Flow\n";
        cout << "2. Identify Bottlenecks\n";
        cout << "3. Crop Suggestions Report\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter query choice: ";
        cin >> query_choice;

        switch (query_choice) {
            case 1:
                calculate_specific_transfer_cost(places, graph);
                break;
            case 2:
                identify_bottlenecks(places, graph);
                break;
            case 3:
                for (const auto& p : places) {
                    if (p.soil_type != "None" && p.deficit_or_surplus < 0) {
                        suggest_crops(p, graph);
                    }
                }
                break;
            case 4:
                cout << "Returning to Main Menu." << endl;
                break;
            default:
                cout << "Invalid query choice." << endl;
        }
    } while (query_choice != 4);
}
