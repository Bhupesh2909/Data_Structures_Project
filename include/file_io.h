#ifndef FILE_IO_H
#define FILE_IO_H

#include "data_structures.h"
using namespace std;

/**
 *  Reads place and connection data from a file, adhering to the required format.
 *  filename The name of the input file.
 *  places Vector to store loaded Place objects.
 *  connections Vector to store loaded connection tuples.
 *  true if data loaded successfully, false otherwise.
 */
bool load_data_from_file(const string& filename, vector<Place>& places, ConnectionList& connections);

#endif // FILE_IO_H
