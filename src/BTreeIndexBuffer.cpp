/**
 * @file BTreeIndexBuffer.cpp
 * @brief Implementation file for the BTreeIndexBuffer class.
 */

#include "BTreeIndexBuffer.h"
using namespace std;

BTreeIndexBuffer::BTreeIndexBuffer(int blockSz, int minCap) {
    blockSize = blockSz;
    minimumBlockCapacity = minCap;
}

int BTreeIndexBuffer::read(std::istream &stream, int headerRecordSize, int blockNumber) {
    // Move to location if needed
    if (blockNumber != -1) {
        stream.clear();
        stream.seekg((blockNumber-1) * blockSize + headerRecordSize);
    }

    // check input stream
    if (!stream) return -1;

    // Get current location in stream
    int addr = stream.tellg();

    // Clear current buffer
    clear();

    // Read block into buffer
    vector<char> buf(blockSize);
    stream.read(buf.data(), blockSize);

    // Used to mark index nodes
    if (buf[0] != 'I') {
        return -1;
    }
    // Remove end spaces as they are rewritten in write function
    auto pos = std::find_if_not(buf.rbegin(), buf.rend(), [](char c) { return std::isspace(c) || c == '\n'; }).base();
    buffer.write(buf.data(), std::distance(buf.begin(), pos)+1);

    // check stream
    if (stream.bad()) {
        stream.clear();
        return -1;
    }

    return addr;
}

int BTreeIndexBuffer::write(std::ostream &stream, int headerRecordSize, int blockNumber) {
    // Move to location if needed
    if (blockNumber != -1) {
        stream.seekp((blockNumber-1) * blockSize + headerRecordSize);
    }

    // Get output location
    int addr = stream.tellp();

    // Write the buffer
    string metadata = "I\n";
    string str = metadata + buffer.str();

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

int BTreeIndexBuffer::unpack(std::vector<int>& separators, std::vector<int>& RBNs) {
    std::string buf = buffer.str();

    size_t semicolonPos = buf.find(';');
    if (semicolonPos == std::string::npos) {
        return -1;
    }

    std::string keysStr = buf.substr(2, semicolonPos);
    std::string RBNsStr = buf.substr(semicolonPos + 1);

    auto splitAndConvertToInt = [](const std::string& str, char delimiter) {
        std::vector<int> result;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            result.push_back(std::stoi(item));
        }
        return result;
    };

    separators = splitAndConvertToInt(keysStr, ',');
    RBNs = splitAndConvertToInt(RBNsStr, ',');

    return 0;
}

int BTreeIndexBuffer::pack(std::vector<int> seperators, std::vector<int> RBNs) {

    string buf;

    for (int i = 0; i < seperators.size(); i++) {
        buf += to_string(seperators[i]);
        if (i < seperators.size()-1) {
            buf += ",";
        }
    }

    buf += ";";

    for (int i = 0; i < RBNs.size(); i++) {
        buf += to_string(RBNs[i]);
        if (i < RBNs.size()-1) {
            buf += ",";
        }
    }

    buf += "\n";

    if (buffer.str().size() +  buf.size() <= blockSize) {
        buffer << buf;
        return 0;
    }

    return -1;
}

void BTreeIndexBuffer::clear() {
    buffer.clear();
    buffer.str("");
}
