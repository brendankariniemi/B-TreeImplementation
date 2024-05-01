/**
 * @file Record.cpp
 * @brief Implementation file for the Record struct.
 */

#include "Record.h"
#include <iomanip>
#include <iostream>

using namespace std;

Record::Record(RecordBuffer &buffer) {
    string lat, lon;
    buffer.unpack(ZipCode);
    buffer.unpack(PlaceName);
    buffer.unpack(State);
    buffer.unpack(County);
    buffer.unpack(lat);
    buffer.unpack(lon);
    Lat = stod(lat);
    Long = stod(lon);
}

void Record::display() {
    cout << left << setw(10) << ZipCode << setw(15) << PlaceName << setw(15) << State << setw(15) << County << setw(10) << Lat << setw(10) << Long << endl;
}
