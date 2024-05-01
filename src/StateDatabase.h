/**
 * @file StateDatabase.cpp
 * @brief Header file for the StateDatabase class.
 */

/**
 * @class StateDatabase
 * @brief A class to store a collection of StateExtrema objects.
 * @details StateDatabase class: A database class to store a collection of
 * state extrema objects utilizing a map which links the State IF (Ex: "MN")
 * to the associated StateExtrema object.
 * Features: Provides methods to process a record and display all currently stored StateExtrema objects.
 */

#ifndef ZIPCODE_STATEDATABASE_H
#define ZIPCODE_STATEDATABASE_H

#include <map>
#include "StateExtrema.h"
#include "Record.h"

class StateDatabase {
public:
    /**
    * @brief Processes a record and updates state information.
    *
    * @param[in] record - The Record object to process.
    *
    * @pre record is a fully initialized object with all fields filled.
    * @post StateExtrema object linked by state ID in record object is updated, or new
    * state is added to map, and StateExtrema object initialized with record data.
    */
    void processRecord(const Record &record);

    /**
    * @brief Prints the state information.
    *
    * @pre None.
    * @post All objects in stateInfoMap are displayed by field.
    */
    void printStateInfo(std::string state) const;

private:
    std::map<std::string, StateExtrema> stateInfoMap; /**< Map that links state ID to StateExtrema object. */
};

#endif //ZIPCODE_STATEDATABASE_H
