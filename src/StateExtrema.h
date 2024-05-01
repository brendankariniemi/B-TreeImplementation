/**
 * @file StateExtrema.h
 * @brief Header file for the StateExtrema struct.
 */

/**
 * @struct StateExtrema
 * @brief A struct for storing information about the extrema of a state.
 * @details StateExtrema Struct: Data structure for storing the zipcode and location (latitude or longitude)
 * of the Northernmost, Easternmost, Southernmost, and Westernmost locations in a state.
 */

#ifndef CSCI331_ZIPCODE_STATEEXTREMA_H
#define CSCI331_ZIPCODE_STATEEXTREMA_H

struct StateExtrema {
    std::string eastZip;    /**< Easternmost zip code of state. */
    double eastLong;        /**< Longitude of easternmost zipcode. */

    std::string westZip;    /**< Westernmost zip code of state. */
    double westLong;        /**< Longitude of westernmost zipcode. */

    std::string northZip;   /**< Northernmost zip code of state. */
    double northLat;        /**< Latitude of northernmost zipcode. */

    std::string southZip;   /**< Southernmost zip code of state. */
    double southLat;        /**< Latitude of southernmost zipcode. */
};

#endif //CSCI331_ZIPCODE_STATEEXTREMA_H
