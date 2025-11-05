#include <iostream>
#include <iomanip>
#include <limits> // Required for numeric_limits
#include "data_structures.h"
#include "file_io.h"
#include "analysis.h"

using namespace std;

// Function to handle bad input
void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * Displays place and connection data currently loaded in memory.
 */
void display_data(const vector<Place>& places, const ConnectionList& connections) {
    if (places.empty()) {
        cout << "No data loaded yet." << endl;
        return;
    }
    
    cout << "\n--- PLACE DATA ---\n";
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Balance" 
         << setw(10) << "Priority" << setw(10) << "Soil" << endl;
    for (const auto& p : places) {
        cout << setw(5) << p.id << setw(20) << p.name << setw(10) << p.deficit_or_surplus 
             << setw(10) << p.priority_level << setw(10) << p.soil_type << endl;
    }
    
    cout << "\n--- CONNECTION DATA ---\n";
    cout << setw(5) << "From" << setw(5) << "To" << setw(10) << "Capacity" << setw(10) << "Cost" << endl;
    for (const auto& conn : connections) {
        cout << setw(5) << get<0>(conn) << setw(5) << get<1>(conn) << setw(10) << get<2>(conn) 
             << setw(10) << get<3>(conn) << endl;
    }
}


int main() {
    vector<Place> places;
    ConnectionList connections;
    const string DATA_FILE = "./code/water_data.txt";
    int choice;

    cout << "Welcome to the Modular Water Management System (MCMF) \n";
    cout << "------------------------------------------------------\n";
    cout << "Data must be in '" << DATA_FILE << "'.\n";

    do {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Load Data from File (" << DATA_FILE << ")\n";
        cout << "2. Run Distribution Analysis (MCMF)\n";
        cout << "3. Display Current Data\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            clear_input();
            continue;
        }

        switch (choice) {
            case 1:
                load_data_from_file(DATA_FILE, places, connections);
                break;
            case 2:
                run_analysis(places, connections);
                break;
            case 3:
                display_data(places, connections);
                break;
            case 4:
                cout << "Exiting Water Management System. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
