/**
 * @file RecordFile.cpp
 * @brief Implementation file for the RecordFile class.
 */

#include "RecordFile.h"
#include <string>

using namespace std;

RecordFile::RecordFile(HeaderBuffer &hbuf) : headerBuffer(hbuf) {

}

RecordFile::~RecordFile() {
    closeFile();
}

bool RecordFile::openFile(const std::string& dataFile, std::string &recordFile) {
    // Open the length indicated file for writing
    string baseFileName = dataFile.substr(0, dataFile.find_last_of('.'));
    recordFile = baseFileName + "-LENGTH_INDICATED_RECORDS.txt";
    file.open(recordFile.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Add default header to file, then create the data for the file
    int headerSize = headerBuffer.writeHeader(file);
    if (headerSize == -1) return false;
    int count = createLengthIndicatedFile(dataFile);
    if (count == -1) return false;

    // Rewrite the header with relevant info
    headerBuffer.fileType = "length indicated records";
    headerBuffer.recordCount = count;
    headerSize = headerBuffer.writeHeader(file);

    // Close the file for writing and open for reading
    file.clear();
    file.close();
    file.open(recordFile.c_str(), std::ios::in | std::ios::binary);

    // Move past the header
    file.seekg(headerSize);
    file.seekp(headerSize);

    if (!file) {
        return false;
    }

    return true;
}

bool RecordFile::closeFile() {
    if (file.is_open()) {
        headerBuffer.stale = "false";
        headerBuffer.writeHeader(file);
        file.close();
        return true;
    }
    return false;
}

int RecordFile::read(RecordBuffer &recordBuffer, int recaddr) {
    return recordBuffer.read(file, recaddr);
}

int RecordFile::write(RecordBuffer &recordBuffer, int recaddr) {
    return recordBuffer.write(file, recaddr);
}

int RecordFile::createLengthIndicatedFile(const string &inputCsvFile) {
    string line;
    int count = 0;

    ifstream infile(inputCsvFile);
    if (!infile) {
        return -1;
    }

    // Handle header
    if (!infile.eof()) {
        getline(infile, line);
    }

    // Make records length indicated
    while (getline(infile, line)) {
        // Write the length of the record followed by the record itself
        file << line.size() + 1 << line;
        count++;

        // Check if it's not the last line, then add a newline
        if (!infile.eof()) {
            file << '\n';
        }
    }

    infile.close();

    return count;
}