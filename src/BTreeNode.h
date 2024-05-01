
#ifndef CSCI331_PROJECT3_BTREENODE_H
#define CSCI331_PROJECT3_BTREENODE_H

#include <fstream>
#include "BTreeIndexBuffer.h"
#include "BlockBuffer.h"
#include "Record.h"

class BTreeNode {
public:
    /**
    * @brief This is the constructor for the BtreeNodes
    * @param maxKeys the maximum number of keys per node
    * @post class object is initialized
    */
    BTreeNode(int maxKeys);

    /**
    * @brief This function reads the node from file
    * @param stream the stream to read from
    * @param headerRecordSize the size of header record
    * @param RBN the block to read
    * @return  Returns -1 if there's an error, otherwise node is filled with data
    */
    int read(std::istream& stream, int headerRecordSize, int RBN);

    /**
    * @brief This function writes the node to the file
    * @param stream the stream to write to
    * @param headerRecordSize the size of header record
    * @param RBN the block to write to
    * @return  Returns -1 if there's an error, otherwise node is written to stream
    */
    int write(std::ostream& stream, int headerRecordSize, int RBN);

    /**
    * @brief This function inserts a record into the B tree
    * @param recordBuffer the record to insert
    * @return -1 on error, 0 otherwise
    */
    int insertRecord(RecordBuffer& recordBuffer);

    /**
    * @brief This function removes a record from the B tree
    * @param recordBuffer the record to remove
    * @return -1 on error, 0 otherwise
    */
    int removeRecord(RecordBuffer& recordBuffer);

    /**
    * @brief This function retrieves a record from the current buffer
    * @param recordBuffer is the buffer object to store record in.
    * @param key is the zipcode to search for.
    * @return Returns the status code as an integer
    */
    int retrieveRecord(RecordBuffer& recordBuffer, int key);

    /**
    * @brief This function inserts a key into the key vector
    * @param key the key to add
    * @param child1 the child to add
    * @param child2 the child to add
    * @return -1 if failed, 0 otherwise
    */
    int insertKeyAndChildren(int key, int child1, int child2 = -1);

    /**
    * @brief This function removes a key into the key vector
    * @param key the key to remove
    * @param child the child to remove
    * @return -1 if failed, 0 otherwise
    */
    int removeKeyAndChildren(int key, int child);

    /**
    * @brief This function prints the node to the output stream
    * @param stream the stream to print node to
    * @return nothing
    */
    void print(std::ostream & stream);

    /**
    * @brief This function shifts a node and it's children toward the root until the B tree becomes balanced
    * @param newNode the node to place half of data into
    * @return -1 if failed, 0 otherwise
    */
    int split(BTreeNode * newNode);

    /**
    * @brief This function merges two nodes together
    * @param fromNode
    * @return -1 if failed, 0 otherwise
    */
    int merge(BTreeNode * fromNode);

    /**
    * @brief This function returns the next node down the tree towards specific key
    * @param key the key to move down the tree towards.
    * @return The next child, an RBN.
    */
    int getNextChild(int key);

    /**
    * @brief Returns all stored children
    * @return a vector containing all children
    */
    std::vector<int> getChildren();

    /**
    * @brief This function returns the largest key in the key vector
    * @return the largest key in node
    */
    int getLargestKey();

    /**
    * @brief This function returns whether a node is a leaf
    * @return True is node is leaf false otherwise
    */
    bool getIsLeaf();

    /**
    * @brief This function sets a node's isLeaf value to true or false
    * @param isL, boolean value to set isLeaf too
    * @return nothing
    */
    void setIsLeaf(bool isL);

    /**
    * @brief This function returns the node's current RBN value
    * @return the RBN value as integer
    */
    int getCurRBN();

    /**
    * @brief Sets the current rbn value
    * @param rbn value to set curRBN to
    * @return nothing
    */
    void setCurRBN(int rbn);

    /**
    * @brief This function returns the node's prev RBN value
    * @return the RBN value as integer
    */
    int getPrevRBN();

    /**
    * @brief Sets the previous rbn value
    * @param rbn value to set prevRBN to
    * @return nothing
    */
    void setPrevRBN(int rbn);

    /**
    * @brief This function returns the node's next RBN value
    * @return the RBN value as integer
    */
    int getNextRBN();

    /**
    * @brief Sets the next rbn value
    * @param rbn value to set nextRBN to
    * @return nothing
    */
    void setNextRBN(int rbn);

    /**
    * @brief This function returns whether a node is overfilled.
    * @return True if its too full, false otherwise
    */
    bool isOverFilled();

    /**
    * @brief This function returns whether a record is too large.
    * @return true if underfilled, false otherwise
    */
    bool isUnderFilled();

private:
    BlockBuffer blockBuffer;           /**< Stores the reference to a block buffer object */
    BTreeIndexBuffer bTreeIndexBuffer; /**< Stores the reference to a index buffer object */
    int curRBN;                        /**< Current RBN in file */
    int maxKeys;                       /**< max number of keys to hold */
    int minKeys;                       /**< min number of keys to hold */
    int numKeys;                       /**< current number of keys stored */
    bool isLeaf;                       /**< stores whether node is a leaf or not */
    std::vector<int> keys;             /**< Stores the keys of node */
    std::vector<int> children;         /**< Stores the children of node */
};

#endif //CSCI331_PROJECT3_BTREENODE_H

