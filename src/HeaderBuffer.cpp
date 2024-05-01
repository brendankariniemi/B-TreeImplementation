/**
 * @file HeaderBuffer.cpp
 * @brief Implementation file for the HeaderBuffer class.
 */

#include "HeaderBuffer.h"
#include <string>

using namespace std;

HeaderBuffer::HeaderBuffer() {
    this->fileType = "";
    this->version = "2.0";
    this->headerRecordSize = 512;
    this->recordSizeDigits = 2;
    this->recordSizeFormat = "ASCII";
    this->blockSize = 512;
    this->minimumBlockCapacity = 256;
    this->recordCount = -1;
    this->blockCount = -1;
    this->recordFieldCount = 6;
    this->recordFieldsType = "STRING";
    this->recordFormat = "ZipCode,PlaceName,State,County,Lat,Long";
    this->recordPrimaryKey = 1;
    this->rbnAvail = 2;
    this->rbnActive = 1;
    this->stale = "true";
}

int HeaderBuffer::readHeader(std::istream &stream)
{
    string line;

    while (std::getline(stream, line) && line != "END")
    {

        int equalPos = line.find('=');
        if (equalPos != std::string::npos)
        {
            string key = line.substr(0, equalPos);
            string value = line.substr(equalPos + 1);

            if (key == "FILE_TYPE")
            {
                fileType = value;
            }
            else if (key == "VERSION")
            {
                version = value;
            }
            else if (key == "HEADER_RECORD_SIZE")
            {
                headerRecordSize = stoi(value);
            }
            else if (key == "RECORD_SIZE_DIGITS")
            {
                recordSizeDigits = stoi(value);
            }
            else if (key == "RECORD_SIZE_FORMAT")
            {
                recordSizeFormat = value;
            }
            else if (key == "BLOCK_SIZE")
            {
                blockSize = stoi(value);
            }
            else if (key == "MINIMUM_BLOCK_CAPACITY")
            {
                minimumBlockCapacity = stoi(value);
            }
            else if (key == "RECORD_COUNT")
            {
                recordCount = stoi(value);
            }
            else if (key == "BLOCK_COUNT")
            {
                blockCount = stoi(value);
            }
            else if (key == "RECORD_FIELD_COUNT")
            {
                recordFieldCount = stoi(value);
            }
            else if (key == "RECORD_FIELDS_TYPE")
            {
                recordFieldsType = value;
            }
            else if (key == "RECORD_FORMAT")
            {
                recordFormat = value;
            }
            else if (key == "RECORD_PRIMARY_KEY")
            {
                recordPrimaryKey = stoi(value);
            }
            else if (key == "RBN_AVAIL")
            {
                rbnAvail = stoi(value);
            }
            else if (key == "RBN_ACTIVE")
            {
                rbnActive = stoi(value);
            }
            else if (key == "STALE")
            {
                stale = value;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }

    // Return the stream position after reading the header
    return stream.tellg();
}

int HeaderBuffer::writeHeader(std::ostream &stream) const
{
    string buffer;
    stream.seekp(0, std::ios::beg);

    buffer += "FILE_TYPE="; buffer += fileType; buffer += '\n';
    buffer += "VERSION="; buffer += version; buffer += '\n';
    buffer += "HEADER_RECORD_SIZE="; buffer += to_string(headerRecordSize); buffer += '\n';
    buffer += "RECORD_SIZE_DIGITS="; buffer += to_string(recordSizeDigits); buffer += '\n';
    buffer += "RECORD_SIZE_FORMAT="; buffer += recordSizeFormat; buffer += '\n';
    buffer += "BLOCK_SIZE="; buffer += to_string(blockSize); buffer += '\n';
    buffer += "MINIMUM_BLOCK_CAPACITY="; buffer += to_string(minimumBlockCapacity); buffer += '\n';
    buffer += "RECORD_COUNT="; buffer += to_string(recordCount); buffer += '\n';
    buffer += "BLOCK_COUNT="; buffer += to_string(blockCount); buffer += '\n';
    buffer += "RECORD_FIELD_COUNT="; buffer += to_string(recordFieldCount); buffer += '\n';
    buffer += "RECORD_FIELDS_TYPE="; buffer += recordFieldsType; buffer += '\n';
    buffer += "RECORD_FORMAT="; buffer += recordFormat; buffer += '\n';
    buffer += "RECORD_PRIMARY_KEY="; buffer += to_string(recordPrimaryKey); buffer += '\n';
    buffer += "RBN_AVAIL="; buffer += to_string(rbnAvail); buffer += '\n';
    buffer += "RBN_ACTIVE="; buffer += to_string(rbnActive); buffer += '\n';
    buffer += "STALE="; buffer += stale; buffer += '\n';
    buffer += "END"; buffer += '\n';

    int remainingSpace = headerRecordSize - buffer.length() - 1;
    buffer += string(remainingSpace, ' '); buffer += '\n';

    // Write the header to file
    stream << buffer;

    // Return header size
    return headerRecordSize;
}
