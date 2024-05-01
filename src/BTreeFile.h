/**
 * @file BTreeFile.h
 * @brief Header file for the BTreeFile class.
 */

/**
 * @class BTreeFile
 * @brief A class for building the Btree File.
 * @details: This class provides methods for working with a file associated with a BlockBuffer.
 * Includes: Methods for opening, closing, reading, creating ,and writing to the file.
 * Assumes:The provided BlockBuffer and HeaderBuffer objects are correctly initialized and valid.
 */

#ifndef PROJECT2_BTREEFILE_H
#define PROJECT2_BTREEFILE_H

#include "RecordBuffer.h"
#include "HeaderBuffer.h"
#include "BTreeNode.h"
#include "RecordFile.h"
#include "StateDatabase.h"
#include <fstream>

class BTreeFile
{
public:
    /**
    * @brief This is the constructor for the BtreeFile, it takes int the header buffer object
    * @param HeaderBuffer Object
    * @param order the order of the b tree
    * @post Class is initialized.
    */
    BTreeFile(HeaderBuffer &hbuf, int order);

    /**
    * @brief This is the destructor for the btree object.
    * @post Object is properly closed.
    */
    ~BTreeFile();

    /**
    * @brief This opens the Btree File.
    * @param bTreeFileName name of b tree file.
    * @return returns int, -1 if error opening the file.
    */
    bool openFile(std::string &bTreeFileName);

    /**
    * @brief inserts a record into the btree
    * @param RecordBuffer record to insert
    * @return returns location of record inserted or -1 if failed
    */
    int insert(RecordBuffer& recordBuffer);

    /**
    * @brief This removes a certain record
    * @param recordBuffer record to remove
    * @return returns location of record removed, or -1 if failed
    */
    int remove(RecordBuffer& recordBuffer);

    /**
    * @brief Searches the b tree for the record matching the key.
    * @param recordBuffer the object to store record in if found.
    * @param key the zipcode to find.
    * @return -1 on search failed, 0 otherwise
    */
    int search(RecordBuffer& recordBuffer, int key);

    /**
    * @brief Display the tree sequence set
    * @param ostream the stream to display too
    * @return none
    */
    void displaySequenceSet(std::ostream& ostream);

    /**
    * @brief Display the extrema to output stream
    * @param ostream the stream to display too
    * @return none
    */
    void displayExtrema(std::ostream& ostream, std::string state);

    /**
    * @brief Display the tree hierarchically
    * @param ostream the stream to display too
    * @return none
    */
    void displayTree(std::ostream& ostream);

private:
    HeaderBuffer &headerBuffer; /**< Stores the reference to the HeaderBuffer object */
    std::fstream file;          /**< Stores the fstream object to the file */
    std::string filename;       /**< Stores the file name for the Btree */
    BTreeNode* root;            /**< This is a pointer the the Btree root Node */
    int order;                  /**< This is the order of the btree*/
    int height;                 /**< This is the height of the btree*/

    /**
    * @brief This function closes the file.
    * @return returns false if error closing the file.
    */
    bool closeFile();

    /**
    * @brief This function adds key pairs to reference block, but handles split root
    * @param largestKey, this is the largest key in the block,
    * @param leaf cantaining first half of records. Is the original block.
    * @param newLeaf leaf containing other half of records
    */
    void handleRootSplit(int largestKey, BTreeNode* leaf, BTreeNode* newLeaf);

    /**
    * @brief This function adds key pairs to reference block in cases not involving root node.
    * @param largestKey, this is the largest key in the block,
    * @param leaf cantaining first half of records. Is the original block.
    * @param newLeaf leaf containing other half of records
    */
    void handleNonRootSplit(int largestKey, BTreeNode* leaf, BTreeNode* newLeaf);

    /**
    * @brief This function handles the merge between a leaf and another leaf.
    * @param parent the parent of leaf to merge
    *@param leaf the leaf to merge with another node
    */
    void handleMerge(BTreeNode *parent, BTreeNode *leaf);

    /**
    * @brief Find Parent Node Returns parent node if you place in child node
    * @param childNode the node of which to  find parent of
    * @return BtreeNode object, parent node of parameter
    */
    BTreeNode* findParentNode(BTreeNode* childNode);

    /**
    * @brief this will find the leaf node based on a key input
    * @param key int, This is a zipcode key
    * @return Returns the leaf node based on the key parameter
    */
    BTreeNode* findLeafNode(int key);

    /**
    * @brief Displays the node to the output stream.
    * @param node, the node to display
    * @param ostream, the stream to display too.
    * @param level, the current level the node is on
    * @param prefix, the prefix used to indent node based on level (for tree appearance)
    * @return nothing
    */
    void displayNode(BTreeNode* node, std::ostream& ostream, int level, const std::string& prefix);

    /**
    * @brief  This flushes all the data from the file and places it to btree
    * @return  Returns False if flush fails, returns True is flush succeeds and file is open
    */
    bool flushData();

};

#endif // PROJECT2_BTREEFILE_H