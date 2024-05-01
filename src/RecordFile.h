/**
 * @file RecordFile.h
 * @brief Header file for the RecordFile class.
 */

/**
 * @class RecordFile
 * @brief A class for managing the file for a recordBuffer.
 * @details: This class provides methods for working with a file associated with a RecordBuffer.
 * Includes: Methods for opening, closing, reading, and writing to the file.
 * Assumes:The provided RecordBuffer and HeaderBuffer objects are correctly initialized and valid.
 */

#ifndef PROJECT2_PART1_RECORDFILE_H
#define PROJECT2_PART1_RECORDFILE_H

#include "RecordBuffer.h"
#include "HeaderBuffer.h"

class RecordFile
{
public:
    /**
     * @brief Constructor for RecordFile class.
     * @param rbuf The RecordBuffer associated with the file.
     * @param hbuf The HeaderBuffer containing file header information.
     */
    RecordFile(HeaderBuffer &hbuf);

    /**
    * @brief This is the destructor for the record file object.
    * @post Object is properly closed.
    */
    ~RecordFile();

    /**
     * @brief Opens the file with the given dataFile.
     * @param dataFile The name of the file to open.
     * @return True if the file is opened successfully, false otherwise.
     */
    bool openFile(const std::string &dataFile, std::string &recordFile);

    /**
     * @brief Reads a record from the file at the specified address.
     * @param recordBuffer The address of the record to read (default: -1).
     * @param recaddr The record address in the file (default: -1).
     * @return 0 if successful, -1 if an error occurs.
     */
    int read(RecordBuffer &recordBuffer, int recaddr = -1);

    /**
     * @brief Writes a record to the file at the specified address.
     * @param recaddr The address of the record to write (default: -1).
     * @param recaddr The record address in the file (default: -1).
     * @return 0 if successful, -1 if an error occurs.
     */
    int write(RecordBuffer &recordBuffer, int recaddr = -1);

private:
    HeaderBuffer &headerBuffer; ///< The HeaderBuffer containing file header information.
    std::fstream file;          ///< The file stream for the record file.

    /**
    * @brief Closes the currently open file.
    * @return True if the file is closed successfully, false otherwise.
    */
    bool closeFile();

    /**
     * @brief Creates a length-indicated file from the given CSV input file.
     * @param inputCsvFile The name of the input CSV file.
     * @return number of records, or -1 on error.
     */
    int createLengthIndicatedFile(const std::string &inputCsvFile);
};

#endif // PROJECT2_PART1_RECORDFILE_H
