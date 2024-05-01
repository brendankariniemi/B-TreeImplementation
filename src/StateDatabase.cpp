/**
 * @file StateDatabase.cpp
 * @brief Implementation file for the StateDatabase class.
 */

#include <iostream>
#include <iomanip>
#include "StateDatabase.h"

using namespace std;

void StateDatabase::processRecord(const Record &record) {
    auto it = stateInfoMap.find(record.State);

    if (it != stateInfoMap.end()) {
        StateExtrema &extrema = it->second;

        if (record.Long < extrema.eastLong) {
            extrema.eastZip = record.ZipCode;
            extrema.eastLong = record.Long;
        }

        if (record.Long > extrema.westLong) {
            extrema.westZip = record.ZipCode;
            extrema.westLong = record.Long;
        }

        if (record.Lat > extrema.northLat) {
            extrema.northZip = record.ZipCode;
            extrema.northLat = record.Lat;
        }

        if (record.Lat < extrema.southLat) {
            extrema.southZip = record.ZipCode;
            extrema.southLat = record.Lat;
        }
    } else {
        StateExtrema extrema;
        extrema.eastZip = record.ZipCode;
        extrema.eastLong = record.Long;
        extrema.westZip = record.ZipCode;
        extrema.westLong = record.Long;
        extrema.northZip = record.ZipCode;
        extrema.northLat = record.Lat;
        extrema.southZip = record.ZipCode;
        extrema.southLat = record.Lat;
        stateInfoMap[record.State] = extrema;
    }
}

void StateDatabase::printStateInfo(std::string state) const {
    if (state == "*") {
        cout << left << setw(5) << "State " << setw(15) << "Easternmost" << setw(15)
             << "Westernmost" << setw(15) << "Northernmost" << setw(15) << "Southernmost" << endl;

        for (auto state: stateInfoMap) {
            cout << left << setw(6) << state.first
                 << setw(15) << state.second.eastZip
                 << setw(15) << state.second.westZip
                 << setw(15) << state.second.northZip
                 << setw(15) << state.second.southZip << endl;
        }
    } else {
        auto it = stateInfoMap.find(state);
        if (it != stateInfoMap.end()) {
            cout << left << setw(5) << "State " << setw(15) << "Easternmost" << setw(15)
                 << "Westernmost" << setw(15) << "Northernmost" << setw(15) << "Southernmost" << endl;

            StateExtrema extrema = it->second;
            cout << left << setw(6) << it->first
                 << setw(15) << extrema.eastZip
                 << setw(15) << extrema.westZip
                 << setw(15) << extrema.northZip
                 << setw(15) << extrema.southZip << endl;
        } else {
            cout << "STATE: " << state << " not found in database!" << endl;
        }
    }
}
