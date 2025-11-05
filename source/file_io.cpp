#include "file_io.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * Reads place and connection data from a file.
 */
bool load_data_from_file(const string& filename, vector<Place>& places, ConnectionList& connections) {
    ifstream file(filename);
    if (!file.is_open()) {
        
        cerr << "ERROR: Could not open file " << filename << ". Please ensure it exists." << endl;
        return false;
    }

    places.clear();
    connections.clear();
    string line;
    int current_id = 0;
    bool reading_places = false;
    bool reading_connections = false;

    cout << "Loading data from " << filename << "..." << endl;

    while (getline(file, line)) {
        
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("[PLACES]", 0) == 0) {
            reading_places = true;
            reading_connections = false;
            continue;
        } else if (line.rfind("[CONNECTIONS]", 0) == 0) {
            reading_places = false;
            reading_connections = true;
            continue;
        }
        
        stringstream ss(line);

        if (reading_places) {
            string name, soil;
            int balance, priority;
            
            if (ss >> name >> balance >> priority >> soil) {
                // Assign ID sequentially based on file order
                places.push_back({current_id++, name, balance, priority, soil});
            } else {
                cerr << "Warning: Skipping malformed PLACES line: " << line << endl;
            }
        } else if (reading_connections) {
            int u, v, capacity, cost;
            if (ss >> u >> v >> capacity >> cost) {
                // FIX: Check that IDs are non-negative and explicitly cast places.size() to int 
                // for safe comparison with signed IDs (u and v).
                if (u >= 0 && v >= 0 && u < (int)places.size() && v < (int)places.size()) {
                    connections.emplace_back(u, v, capacity, cost);
                } else {
                    // Updated error message to reflect the current maximum ID count
                    cerr << "Warning: Invalid Place ID in CONNECTIONS line: " << line 
                         << " (IDs must be non-negative and less than " << places.size() << ")" << endl;
                }
            } else {
                cerr << "Warning: Skipping malformed CONNECTIONS line: " << line << endl;
            }
        }
    }

    
    cout << "Data loaded successfully. " << places.size() << " places and " 
         << connections.size() << " connections found." << endl;
    return true;
}
