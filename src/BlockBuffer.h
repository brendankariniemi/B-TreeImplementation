/**
 * @file BlockBuffer.h
 * @brief Header file for the BlockBuffer class.
 */

/**
 * @class BlockBuffer
 * @brief A class that has functions to read, write, pack, and unpack block files.
 * @details:
 * Includes: The ability to read and write blocks one block at a time.
 * Assumes: all references to other buffers to be correct and working.
 */

#ifndef CSCI331_PROJECT2_P2_BLOCKBUFFER_H
#define CSCI331_PROJECT2_P2_BLOCKBUFFER_H

#include <iostream>
#include <sstream>
#include "RecordBuffer.h"

class BlockBuffer {
public:
    /**
    * @brief BlockBuffer Constructor.
    * @param blockSz Size as an integer.
    * @param minCap minimum block capacity
    * @post Class is initialized
    */
    BlockBuffer(int blockSz = 512, int minCap = 256);

    /**
    * @brief Read Function, reads one block of data and stores it in the buffer at a time.
    * @param  stream the input to read from.
    * @param  headerRecordSize the size of the header record of file reading from.
    * @param  blockNumber the relative blocknumber to read the block from.
    * @return -1 if error, non error the address of of stream location.
    */
    int read(std::istream & stream, int headerRecordSize, int blockNumber = -1);

    /**
    * @brief Write Function, writes the data currently stored in buffer.
    * @param  stream the output to write to.
    * @param  headerRecordSize the size of the header record of file writing to.
    * @param  blockNumber the relative blocknumber to write the block to.
    * @return -1 if error, non error the address of of stream location.
    */
    int write(std::ostream & stream, int headerRecordSize, int blockNumber = -1);

    /**
    * @brief Unpack Function.
    * @param rBuf The record buffer to unpack data into.
    * @return int value of address where record is read.
    */
    int unpack(RecordBuffer &rBuf);

    /**
    * @brief Pack Function.
    * @param rBuf The record buffer to pack data from.
    * @return int value of address of where the record was written.
    */
    int pack(RecordBuffer &rBuf);

    /**
    * @brief Clear Function, clears buffer.
    * @return nothing.
    */
    void clear();

    /**
    * @brief Getter Function for NextRBN variable.
    * @return integer value of NextRBN variable.
    */
    int getNextRBN();

    /**
    * @brief Getter Function for PrevRBN variable.
    * @return integer value of PrevRBN variable.
    */
    int getPrevRBN();

    /**
    * @brief Getter Function for curRBN variable.
    * @return integer value of curRBN variable.
    */
    int getCurRBN();

    /**
    * @brief Getter Function for NumRecords variable.
    * @return integer value of NumRecords variable.
    */
    int getNumRecords();

    /**
    * @brief Setter Function for NextRBN variable.
    * @param integer value of NextRBN variable.
    */
    void setNextRBN(int rbn);

    /**
    * @brief Setter Function for PrevRBN variable.
    * @return integer value of PrevRBN variable.
    */
    void setPrevRBN(int rbn);

    /**
    * @brief Setter Function for CurRBN variable.
    * @return integer value of CurRBN variable.
    */
    void setCurRBN(int rbn);

    /**
    * @brief Setter Function for NumRecords variable.
    * @return integer value of NumRecords variable.
    */
    void setNumRecords(int num);

    /**
    * @brief Checks if buffer is too full.
    * @return true if buffer contains more bytes than specified block size, false otherwise.
    */
    bool isOverFilled();

    /**
    * @brief Checks if buffer is under minimum requirements.
    * @return true if buffer contains less bytes than specified minimum block, false otherwise.
    */
    bool isUnderFilled();

    /**
    * @brief Splits the records of the current buffer into the passed in buffer.
    * @param newBlockBuffer the new block buffer to place half the upper half of records into.
    * @return nothing.
    */
    void splitBuffer(BlockBuffer &newBlockBuffer);

    /**
    * @brief Merges the records from passed in buffer into the current buffer.
    * @param newBlockBuffer the new block buffer to get data to merge.
    * @return nothing.
    */
    void mergeBuffer(BlockBuffer &newBlockBuffer);

    /**
    * @brief Redistributes the records from passed in buffer into the current buffer until min capacity is reached.
    * @param newBlockBuffer the new block buffer to get data to redistribute.
    * @return nothing.
    */
    void redistributeBuffer(BlockBuffer &newBlockBuffer);

    /**
    * @brief Gets the largest key from the buffer.
    * @return The int value of the largest key in buffer.
    */
    int getLargestKey();

    /**
    * @brief Sorts the buffer based on key
    * @return -1 on error, 0 otherwise
    */
    int sortBuffer();

    /**
    * @brief Removes a record from the buffer
    * @param key The record to remove
    * @return -1 on error, 0 otherwise
    */
    int removeRecord(int key);

private:
    std::stringstream buffer; /**< Used to help in the pack and unpack functions */
    int blockSize;            /**< Stores the block size as int */
    int minimumBlockCapacity; /**< Stores the minimum block size as int */
    int numRecords;           /**< Stores the Number of Records as int */
    int prevRBN;              /**< Keeps state of block next block number to read */
    int nextRBN;              /**< Keeps state of block previous block number read */
    int curRBN;               /**< Keeps state of block current block number */
};


#endif //CSCI331_PROJECT2_P2_BLOCKBUFFER_H