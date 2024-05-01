/**
 * @file BlockBuffer.cpp
 * @brief Implementation file for the BlockBuffer class.
 */

#include "BlockBuffer.h"
#include <sstream>
using namespace std;

BlockBuffer::BlockBuffer(int blockSz, int minCap) {
    blockSize = blockSz;
    minimumBlockCapacity = minCap;
    prevRBN = 0;
    nextRBN = 0;
    curRBN = 0;
    numRecords = 0;
}

int BlockBuffer::read(std::istream &stream, int headerRecordSize, int blockNumber) {
    // Move to location if needed
    if (blockNumber != -1) {
        stream.clear();
        stream.seekg((blockNumber-1) * blockSize + headerRecordSize);
    }

    // check input stream
    if (!stream) return -1;

    // Get current location in stream
    int addr = stream.tellg();
    curRBN = (addr + blockSize - headerRecordSize) / blockSize;

    // Clear current buffer
    clear();

    // Read block into buffer
    vector<char> buf(blockSize);
    stream.read(buf.data(), blockSize);
    // Easy way to check if block is completely empty
    if (buf[0] == '\0') return -1;
    // Remove end spaces as they are rewritten in write function
    auto pos = std::find_if_not(buf.rbegin(), buf.rend(), [](char c) { return std::isspace(c) || c == '\n'; }).base();
    // copy data into internal buffer
    buffer.write(buf.data(), std::distance(buf.begin(), pos)+1);

    // Get metadata
    string metadata;
    getline(buffer, metadata, ',');
    numRecords = stoi(metadata);
    std::getline(buffer, metadata, ',');
    prevRBN = stoi(metadata);
    std::getline(buffer, metadata);
    nextRBN = std::stoi(metadata);
    buffer.seekg(0);

    // check stream
    if (stream.bad()) {
        stream.clear();
        return -1;
    }

    return addr;
}

int BlockBuffer::write(std::ostream &stream, int headerRecordSize, int blockNumber) {

    // Move to location if needed
    if (blockNumber != -1) {
        stream.seekp((blockNumber-1) * blockSize + headerRecordSize);
    }

    // Get output location
    int addr = stream.tellp();
    curRBN = (addr + blockSize - headerRecordSize) / blockSize;

    // Write the buffer - always rewrite metadata
    string str = buffer.str();
    int pos = str.find_first_of('\n');
    if (pos > 0) str = str.substr(pos+1);
    str = to_string(numRecords) + "," + to_string(prevRBN) + "," + to_string(nextRBN) + "\n" + str;


    int sz = str.length();
    int remainingSpace = blockSize - sz;
    if (remainingSpace > 0) {
        str += string(remainingSpace-1, ' '); str += '\n';
    }
    stream.write(str.c_str(), blockSize);

    // check stream
    if (!stream) return -1;

    return addr;
}

int BlockBuffer::unpack(RecordBuffer &rBuf) {
    numRecords--;
    if (numRecords < 0) {
        numRecords = 0;
        return -1;
    }

    // Ignore block metadata
    string buf = buffer.str();
    int pos = buf.find_first_of('\n');
    if (pos > 0) buf = buf.substr(pos+1);
    buffer.seekg(pos+1);

    // Read the record into record buffer, than remove from block buffer
    int recordAddr = rBuf.read(buffer);
    int curAddr = buffer.tellg();
    buf.erase(0, curAddr - recordAddr);
    clear();

    // Rewrite new block buffer
    buffer << numRecords << "," << prevRBN << "," << nextRBN << endl;
    buffer << buf;

    return recordAddr;
}

int BlockBuffer::pack(RecordBuffer &rBuf) {
    numRecords++;
    string buf = buffer.str();
    // Get the buffer without the current block metadata as it will be rewritten
    int pos = buf.find_first_of('\n');
    if (pos > 0) buf = buf.substr(pos+1);
    clear();
    buffer << numRecords << "," << prevRBN << "," << nextRBN << endl;
    buffer << buf;

    return rBuf.write(buffer);
}

void BlockBuffer::clear() {
    buffer.clear();
    buffer.str("");
}

int BlockBuffer::getNextRBN() {
    return nextRBN;
}

int BlockBuffer::getPrevRBN() {
    return prevRBN;
}

int BlockBuffer::getCurRBN() {
    return curRBN;
}

int BlockBuffer::getNumRecords() {
    return numRecords;
}

void BlockBuffer::setNextRBN(int rbn) {
    nextRBN = rbn;
}

void BlockBuffer::setPrevRBN(int rbn) {
    prevRBN = rbn;
}

void BlockBuffer::setCurRBN(int rbn) {
    curRBN = rbn;
}

void BlockBuffer::setNumRecords(int num) {
    numRecords = num;
}

bool BlockBuffer::isOverFilled() {
    return buffer.str().length() > blockSize;
}

bool BlockBuffer::isUnderFilled() {
    return buffer.str().length() < minimumBlockCapacity;
}

void BlockBuffer::splitBuffer(BlockBuffer &newBlockBuffer) {
    RecordBuffer rBuf;
    BlockBuffer tmpBuffer1;
    BlockBuffer tmpBuffer2;

    // Assuming splitting the records evenly between the current and new block
    int recordsToMove = numRecords / 2.0;

    // Move the records to the new block buffer
    int num = numRecords;
    for (int i = 0; i < num; i++) {
        unpack(rBuf);
        if (i < recordsToMove) tmpBuffer1.pack(rBuf);
        else tmpBuffer2.pack(rBuf);
    }

    clear();

    while(tmpBuffer1.unpack(rBuf) != -1) {
        pack(rBuf);
    }

    while(tmpBuffer2.unpack(rBuf) != -1) {
        newBlockBuffer.pack(rBuf);
    }

}

void BlockBuffer::mergeBuffer(BlockBuffer &newBlockBuffer) {
    // Move the records to the new block buffer
    int count = newBlockBuffer.getNumRecords();
    for (int i = 0; i < count; ++i) {
        RecordBuffer rBuf;
        newBlockBuffer.unpack(rBuf);
        pack(rBuf);
    }
}

void BlockBuffer::redistributeBuffer(BlockBuffer &newBlockBuffer) {
    while (isUnderFilled()) {
        RecordBuffer rBuf;
        newBlockBuffer.unpack(rBuf);
        pack(rBuf);
    }
}

int BlockBuffer::getLargestKey() {
    // Create a copy of the current buffer
    std::stringstream bufferCopy(this->buffer.str());

    RecordBuffer rbuf;
    int largestKey = -1;
    BlockBuffer tmpBuffer;
    tmpBuffer.read(bufferCopy, 0);

    while (tmpBuffer.unpack(rbuf) != -1) {
        int key = rbuf.getRecordKey();
        if (key > largestKey) {
            largestKey = key;
        }
    }

    return largestKey;
}

int BlockBuffer::sortBuffer() {
    vector<RecordBuffer> recordBuffers;
    RecordBuffer recordBuffer;

    while(unpack(recordBuffer) != -1) {
        recordBuffers.push_back(recordBuffer);
    }

    std::sort(recordBuffers.begin(), recordBuffers.end(),
              [](RecordBuffer& a, RecordBuffer& b) {
                  return a.getRecordKey() < b.getRecordKey();
              });

    clear();

    for (auto rBuf : recordBuffers) {
        pack(rBuf);
    }

    return 0;
}

int BlockBuffer::removeRecord(int key) {
    vector<RecordBuffer> recordBuffers;
    RecordBuffer recordBuffer;

    while(unpack(recordBuffer) != -1) {
        if (recordBuffer.getRecordKey() != key) {
            recordBuffers.push_back(recordBuffer);
        }
    }

    clear();

    for (auto rBuf : recordBuffers) {
        pack(rBuf);
    }
}