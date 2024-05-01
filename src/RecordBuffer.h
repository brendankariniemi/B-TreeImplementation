/**
 * @file RecordBuffer.h
 * @brief Header file for the RecordBuffer class.
 */

/**
 * @class RecordBuffer
 * @brief A class for managing a recordBuffer.
 * @details RecordBuffer class: Data structure for managing a header buffer.
 * Includes: File type, version, data types, record format, index file and more.
 */

#ifndef ZIPCODE_BUFFER_H
#define ZIPCODE_BUFFER_H

#include <string>
#include <fstream>
#include <vector>

class RecordBuffer {
public:
    /**
    * @brief Constructor for RecordBuffer class.
    * @param maxSize The maximum size of the buffer.
    */
    RecordBuffer(int maxSize = 1000);

    /**
    * @brief Read an entire record into the buffer.
    * @param stream the input file stream.
    * @return -1 if error or the current byte address.
    */
    int read(std::istream & stream, int recaddr = -1);

    /**
    * @brief Write an entire record into the buffer.
    * @param stream the outout file stream.
    * @return -1 if error or the number of bytes written.
    */
    int write(std::ostream & stream, int recaddr = -1);

    /**
    * @brief Read one field from the buffer.
    * @param field the field value to read.
    * @return -1 if error or the current byte address.
    */
    int unpack(std::string &field);

    /**
    * @brief Write one field to the buffer.
    * @param field the field value to write.
    * @return -1 if error or the number of bytes written.
    */
    int pack(std::string field);

    /**
    * @brief Clear all buffer data.
    * @return nothing.
    */
    void clear();

    /**
    * @brief Gets the buffer size.
    * @return buffer size as an integer.
    */
    int getBufferSize();

    /**
    * @brief Gets the record key (zipcode).
    * @return key as an integer.
    */
    int getRecordKey();

private:
    std::vector<char> buffer; /**< The buffer object */
    int maxBufferSize;        /**< The max buffer size */
    int nextByte;             /**< The next byte location to read */
    char deliminator;         /**< The delimiting character */
};

#endif //ZIPCODE_BUFFER_H
