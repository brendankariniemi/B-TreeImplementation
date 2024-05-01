/**
 * @file HeaderBuffer.h
 * @brief Header file for the HeaderBuffer class.
 */

/**
 * @class HeaderBuffer
 * @brief A class for managing a buffer for a files header information.
 * @details: This class provides methods to read and write header information from/to an input/output stream.
 * Features: Reads and writes header information from/to input/output streams.
 * Assumptions:Assumes the input stream and output stream provided are valid and open.
 */

#ifndef PROJECT2_PART1_HEADERBUFFER_H
#define PROJECT2_PART1_HEADERBUFFER_H

#include "fstream"

class HeaderBuffer
{

public:
    /**
     * @brief Initialize the header buffer with default values
     */
    HeaderBuffer();

    /**
     * @brief Read the header information from the given input stream.
     * @param stream The input stream from which to read the header.
     * @return 0 if successful, -1 if an error occurs.
     */
    int readHeader(std::istream &stream);

    /**
     * @brief Write the header information to the given output stream.
     * @param stream The output stream to which to write the header.
     * @return 0 if successful, -1 if an error occurs.
     */
    int writeHeader(std::ostream &stream) const;

    std::string fileType;           /**< The structure of the file */
    std::string version;            /**< The version of the file format. */
    int headerRecordSize;           /**< The size of header record. */
    int recordSizeDigits;           /**< The number of digits for the record size. */
    std::string recordSizeFormat;   /**< The format of the record size. */
    int blockSize;                  /**< The size of the blocks in bytes. */
    int minimumBlockCapacity;       /**< The minimum number of bytes in block. */
    int recordCount;                /**< The total count of records. */
    int blockCount;                 /**< The total count of blocks. */
    int recordFieldCount;           /**< The number of fields in each record. */
    std::string recordFieldsType;   /**< The data type of record fields. */
    std::string recordFormat;       /**< The format of the record fields. */
    int recordPrimaryKey;           /**< The primary key of the records. */
    int rbnAvail;                   /**< Link to beginning of available sequence set. */
    int rbnActive;                  /**< Link to beginning of active sequence set. */
    std::string stale;              /**< Indicates if data is stale. */
};

#endif // PROJECT2_PART1_HEADERBUFFER_H
