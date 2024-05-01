/**
 * @file Record.h
 * @brief Header file for the Record struct.
 */

/**
 * @struct Record
 * @brief A struct for storing information pertaining to a zip code record.
 * @details Record Struct: Data structure for storing information about a zip code record.
 * Includes: Zipcode, PlaceName, State, County, Latitude, and Longitude.
 * Assumes: This struct constructor assumes that the input will always have six fields
 */

#ifndef ZIPCODE_RECORD_H
#define ZIPCODE_RECORD_H

#include "RecordBuffer.h"

struct Record {
    /**
    * @brief Default constructor.
    * @param[in] buffer A recordBuffer containing the record information, length indicated and seperated by commas.
    * @pre recordBuffer has six comma seperated fields.
    * @post recordBuffer has been parsed and struct fields have all been filled with data.
    */
    Record(RecordBuffer &buffer);


    /**
    * @brief Display Function.
    * @pre recordBuffer has six comma seperated fields.
    * @post The recordBuffer's fields have been outputted to the terminal.
    */
    void display();

    std::string ZipCode;      /**< ZipCode of record */
    std::string PlaceName;    /**< Place Name of record */
    std::string State;        /**< State of record */
    std::string County;       /**< County of record */
    double Lat;               /**< latitude of record */
    double Long;              /**< Longitude of record */
};

#endif //ZIPCODE_RECORD_H

