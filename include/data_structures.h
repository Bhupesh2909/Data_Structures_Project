#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <vector>
#include <string>
#include <tuple>
#include <climits>
using namespace std;

// Global constant for priority penalization (used in MCMF cost calculation)
const int PRIORITY_PENALTY = 1000; 

// --- Core Network Structures ---

/**
 *  Represents a City or District (Vertex) in the network.
 */
struct Place {
    int id;
    string name;
    int deficit_or_surplus; // Negative for deficit, positive for surplus (in KL/hr)
    int priority_level;     // 1 (Highest) to 5 (Lowest)
    string soil_type;  // e.g., "Clay", "Loam", "Sand", "None"
};

/**
 *  Represents a Road or Pipe (Edge) for the Minimum Cost Maximum Flow (MCMF) algorithm.
 */
struct Edge {
    int to_place;
    int capacity;       // Max capacity of the pipe (KL/hr)
    int flow;           // Current flow of water (set by the solver)
    int cost;           // Base cost per unit of flow ($ per KL)
    int reverse_edge;   // Index of the reverse edge in the destination's adj list
};

/**
 *  Represents the result of the shortest path search.
 */
struct PathResult {
    int flow = 0;
    int cost = 0;
};

// Global network representation (Adjacency List)
typedef vector<vector<Edge>> WaterNetwork;

// --- Crop Suggestion Structures ---

/**
 *  Fixed profile data for crops.
 */
struct CropProfile {
    string name;
    int water_requirement; // 1-5 (Low to High)
    string ideal_soil;
};

// Fixed lookup table for crop data
const vector<CropProfile> CROP_DATA = {
    {"Sorghum (Low Water)", 1, "Sand"},
    {"Cotton (Low/Medium Water)", 2, "Sand"},
    {"Wheat (Medium Water)", 3, "Loam"},
    {"Corn (Medium Water)", 3, "Loam"},
    {"Rice (High Water)", 5, "Clay"},
    {"Sugarcane (High Water)", 4, "Loam"}
};

// Used to store connection data before building the graph:
// tuple<From_ID, To_ID, Capacity, Base_Cost>
typedef vector<tuple<int, int, int, int>> ConnectionList;

#endif // DATA_STRUCTURES_H
