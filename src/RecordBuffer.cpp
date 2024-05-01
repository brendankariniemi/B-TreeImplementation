/**
 * @file RecordBuffer.cpp
 * @brief Implementation file for the RecordBuffer class.
 */

#include "RecordBuffer.h"

using namespace std;

RecordBuffer::RecordBuffer(int maxSize) {
    deliminator = ',';
    maxBufferSize = maxSize;
    nextByte = 0;
}

int RecordBuffer::read(istream &stream, int recaddr) {
    // Move to location if needed
    if (recaddr != -1) {
        stream.clear();
        stream.seekg(recaddr, ios::beg);
        if (stream.tellg() != recaddr) return -1;
    }

    // check stream
    if (!stream) return -1;

    // Get current location in stream
    int addr = stream.tellg();

    clear();

    char rsz[3]; // Make sure it can hold 2 characters and a null character
    stream.read(rsz, 2);
    rsz[2] = '\0'; // Null-terminate the string
    int recordSize = 0;
    try {
        recordSize = std::stoi(rsz);
    }
    catch (...) {
        return -1;
    }

    // Make sure record can fit in recordBuffer
    if (recordSize > maxBufferSize) return -1;
    // read the record into recordBuffer
    buffer.resize(recordSize);
    stream.read(buffer.data(), recordSize);

    // check stream
    if (stream.bad()) {
        stream.clear();
        return -1;
    }

    return addr;
}

int RecordBuffer::write(ostream &stream, int recaddr) {

    // Move to location if needed
    if (recaddr != -1) {
        stream.seekp(recaddr);
        if (stream.tellp() != recaddr) return -1;
    }

    // Get output location
    int addr = stream.tellp();

    // Write the recordBuffer (record) size
    int recordSize;
    recordSize = buffer.size();
    stream << to_string(recordSize);

    // check stream
    if (!stream) return -1;

    // Write the recordBuffer
    stream.write(buffer.data(), buffer.size());

    // check stream
    if (!stream) return -1;

    return addr;
}

int RecordBuffer::unpack(string &field) {

    int bufferIndex = nextByte;

    while (bufferIndex < buffer.size() && buffer[bufferIndex] != deliminator) {
        field.push_back(buffer[bufferIndex]);
        bufferIndex++;
    }

    // deliminator not found
    if (bufferIndex == buffer.size()) return -1;
    nextByte = bufferIndex+1;

    return (int)field.length();
}

int RecordBuffer::pack(string field) {
    // ensure recordBuffer capacity
    if (buffer.size() + field.length() > maxBufferSize) return -1;

    // Add field to recordBuffer
    buffer.insert(buffer.end(), field.begin(), field.end());

    // Add deliminator
    buffer.push_back(deliminator);

    return (int)field.length();
}

void RecordBuffer::clear() {
    buffer.clear();
    nextByte = 0;
}

int RecordBuffer::getBufferSize() {
    return buffer.size() + to_string(buffer.size()).length();
}

int RecordBuffer::getRecordKey() {
    string key;
    int i = 0;

    while (i < buffer.size() && buffer[i] != deliminator) {
        key.push_back(buffer[i]);
        i++;
    }

    return stoi(key);
}


