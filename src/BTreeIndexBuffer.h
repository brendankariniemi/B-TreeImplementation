/**
 * @file BTreeIndexBuffer.h
 * @brief Header file for the BTreeIndexBuffer class.
 */

/**
 * @class BTreeIndexBuffer
 * @brief A class for managing a buffer for a files header information.
 * @details: This class provides methods to read and write header information from/to an input/output stream.
 * Features: Reads and writes header information from/to input/output streams.
 * Assumptions: Assumes the input stream and output stream provided are valid and open.
 */

#ifndef CSCI331_PROJECT4_BTREEINDEXBUFFER
#define CSCI331_PROJECT4_BTREEINDEXBUFFER

#include <iostream>
#include <sstream>
#include "RecordBuffer.h"

class BTreeIndexBuffer {
public:
    /**
     * @brief Constructor for BTreeIndexBuffer.
     * @param blockSz The block size.
     * @param minCap The minimum block capacity.
     */
    BTreeIndexBuffer(int blockSz = 512, int minCap = 256);

    /**
     * @brief Reads index data from an input stream.
     * @param stream The input stream to read from.
     * @param headerRecordSize The size of the header record.
     * @param blockNumber The block number to read.
     * @return The address where the read operation occurred.
     */
    int read(std::istream & stream, int headerRecordSize, int blockNumber = -1);

    /**
     * @brief Writes index data to an output stream.
     * @param stream The output stream to write to.
     * @param headerRecordSize The size of the header record.
     * @param blockNumber The block number to write to.
     * @return The address where the write operation occurred.
     */
    int write(std::ostream & stream, int headerRecordSize, int blockNumber = -1);

    /**
     * @brief Unpacks the buffer content into vectors of separators and RBNs.
     * @param separators Vector to store the separators.
     * @param RBNs Vector to store the RBNs (relative block numbers).
     * @return 0 on success, -1 if the format is incorrect.
     */
    int unpack(std::vector<int>& seperators, std::vector<int>& RBNs);

    /**
     * @brief Packs vectors of separators and RBNs into the buffer.
     * @param separators Vector of separators.
     * @param RBNs Vector of RBNs.
     * @return 0 on success, -1 if the buffer size exceeds the block size.
     */
    int pack(std::vector<int> seperators, std::vector<int> RBNs);

    /**
     * @brief Clears the buffer.
     * @return nothing
     */
    void clear();

private:
    std::stringstream buffer; /**< Used to help in the pack and unpack functions*/
    int blockSize;            /**< Stores the block size as int */
    int minimumBlockCapacity; /**< Stores the minimum block size as int */
    int numSeparators;        /**< Stores the number of separators */
    int lengthSeparators;     /**< Stores the length of separators */
};

#endif //CSCI331_PROJECT4_BTREEINDEXBUFFER
