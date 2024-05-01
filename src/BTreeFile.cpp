/**
 * @file BlockFile.cpp
 * @brief Implementation file for the BlockFile class.
 */

#include "BTreeFile.h"
#include "RecordBuffer.h"
#include <string>
#include <utility>
using namespace std;

BTreeFile::BTreeFile(HeaderBuffer &hbuf, int order) : headerBuffer(hbuf), root(nullptr) {
    this->order = order;
    this->height = 1;
    this->root = new BTreeNode(order);
}

BTreeFile::~BTreeFile() {
    closeFile();
}

bool BTreeFile::openFile(std::string &bTreeFileName) {
    // Attempt to open in read/write
    filename = bTreeFileName;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    // If that fails, most likely means file does not exist, so create it and write header.
    if (!file.is_open()) {
        file.open(filename.c_str(), std::ios::out | std::ios::binary);
        headerBuffer.fileType = "blocked sequence set with index";
        headerBuffer.writeHeader(file);
        flushData();
    } else {
        headerBuffer.readHeader(file);
    }

    // Move past the header
    file.seekg(headerBuffer.headerRecordSize);
    file.seekp(headerBuffer.headerRecordSize);

    // Check stream status
    if (!file) {
        return false;
    }

    // Read root into memory
    if (root->read(file, headerBuffer.headerRecordSize, 1) == -1) {
        flushData();
        // If read fails assume empty file, so init with root
        if (root->write(file, headerBuffer.headerRecordSize, 1) == -1)
        {
            return false;
        }
    }

    return true;
}

bool BTreeFile::closeFile() {
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        headerBuffer.blockCount = file.tellg() / headerBuffer.blockSize;
        headerBuffer.stale = "false";
        headerBuffer.writeHeader(file);
        file.close();
        return true;
    }
    return false;
}

int BTreeFile::insert(RecordBuffer& recordBuffer) {
    int key = recordBuffer.getRecordKey();
    BTreeNode* leaf = findLeafNode(key);

    // If the leaf is too full
    if (leaf->insertRecord(recordBuffer) == -1) {
        BTreeNode* newLeaf = new BTreeNode(order);
        leaf->split(newLeaf);
        int largestKey = leaf->getLargestKey();

        if (leaf == root) {
            handleRootSplit(largestKey, leaf, newLeaf);
        } else {
            handleNonRootSplit(largestKey, leaf, newLeaf);
        }
    } else {
        leaf->write(file, headerBuffer.headerRecordSize, leaf->getCurRBN());
    }

    return flushData();
}

int BTreeFile::remove(RecordBuffer& recordBuffer) {
    int key = recordBuffer.getRecordKey();
    BTreeNode* leaf = findLeafNode(key);
    BTreeNode* parent = findParentNode(leaf);

    int largestKey = leaf->getLargestKey();
    int status = leaf->removeRecord(recordBuffer);

    // Update parent if largest key changed
    int newLargestKey = leaf->getLargestKey();
    if (newLargestKey != largestKey) {
        parent->removeKeyAndChildren(largestKey, leaf->getCurRBN());
        parent->insertKeyAndChildren(newLargestKey, leaf->getCurRBN());
    }

    // If the leaf is under limit
    if (status == -1) {
        handleMerge(parent, leaf);
    } else {
        leaf->write(file, headerBuffer.headerRecordSize, leaf->getCurRBN());
    }

    return flushData();
}

int BTreeFile::search(RecordBuffer& recordBuffer, int key) {
    BTreeNode * node = findLeafNode(key);

    if (node == nullptr) {
        return -1;
    } else {
        node->retrieveRecord(recordBuffer, key);
        if (recordBuffer.getBufferSize() <= headerBuffer.recordSizeDigits) {
            return -1;
        }
    }

    return 0;
}

void BTreeFile::displaySequenceSet(std::ostream &ostream) {
    BlockBuffer blockBuffer;
    RecordBuffer recordBuffer;
    stringstream ss;

    // Get leftmost node
    BTreeNode * node = findLeafNode(0);
    int RBN = node->getCurRBN();

    while(RBN != 0) {
        blockBuffer.read(file, headerBuffer.headerRecordSize, RBN);

        int currentBlock = blockBuffer.getCurRBN();
        cout << "RELATIVE BLOCK NUMBER: " << currentBlock << endl;

        blockBuffer.write(ss, 0);
        cout << ss.str();

        ss.clear();
        ss.str("");
        RBN = blockBuffer.getNextRBN();
    }
}

void BTreeFile::displayExtrema(ostream &ostream, std::string state) {
    BlockBuffer blockBuffer;
    RecordBuffer recordBuffer;
    StateDatabase stateDb;

    // Get leftmost node
    BTreeNode * node = findLeafNode(0);
    int RBN = node->getCurRBN();

    while(RBN != 0) {
        blockBuffer.read(file, headerBuffer.headerRecordSize, RBN);

        while(blockBuffer.unpack(recordBuffer) != -1) {
            Record record(recordBuffer);
            stateDb.processRecord(record);
        }

        RBN = blockBuffer.getNextRBN();
    }

    stateDb.printStateInfo(std::move(state));
}

void BTreeFile::displayTree(ostream &ostream) {
    displayNode(root, ostream, 0, "");
}


void BTreeFile::handleRootSplit(int largestKey, BTreeNode* leaf, BTreeNode* newLeaf) {
    // If the split node was the root, we need to create a new root
    BTreeNode* newRoot = new BTreeNode(order);
    newRoot->insertKeyAndChildren(largestKey, headerBuffer.rbnAvail, headerBuffer.rbnAvail+1);

    leaf->setNextRBN(headerBuffer.rbnAvail + 1);
    leaf->write(file, headerBuffer.headerRecordSize, headerBuffer.rbnAvail);
    headerBuffer.rbnAvail++;

    newLeaf->setPrevRBN(leaf->getCurRBN());
    newLeaf->write(file, headerBuffer.headerRecordSize, headerBuffer.rbnAvail);
    headerBuffer.rbnAvail++;

    root = newRoot;
    root->setIsLeaf(false);
    root->write(file, headerBuffer.headerRecordSize, 1);
    height++;
}

void BTreeFile::handleNonRootSplit(int largestKey, BTreeNode* leaf, BTreeNode* newLeaf) {
    BTreeNode* parent = findParentNode(leaf);
    if (parent == nullptr) {
        handleRootSplit(largestKey, leaf, newLeaf);
        return;
    }

    // Write both child nodes and insert key pair into parent
    parent->insertKeyAndChildren(largestKey, headerBuffer.rbnAvail);

    // Write the split leaf
    newLeaf->setNextRBN(leaf->getNextRBN());
    leaf->setNextRBN(headerBuffer.rbnAvail);
    leaf->write(file, headerBuffer.headerRecordSize, leaf->getCurRBN());

    // Write the new leaf
    newLeaf->setPrevRBN(leaf->getCurRBN());
    newLeaf->write(file, headerBuffer.headerRecordSize, headerBuffer.rbnAvail);
    headerBuffer.rbnAvail++;

    // Set the prev RBN of the next node of new node
    if (newLeaf->getNextRBN() != 0) {
        BTreeNode * nextOfNewLeaf = new BTreeNode(order);
        nextOfNewLeaf->read(file, headerBuffer.headerRecordSize, newLeaf->getNextRBN());
        nextOfNewLeaf->setPrevRBN(newLeaf->getCurRBN());
        nextOfNewLeaf->write(file, headerBuffer.headerRecordSize, nextOfNewLeaf->getCurRBN());
    }

    // Check if the parent is overfull and handle splitting recursively
    if (parent->isOverFilled()) {
        BTreeNode* newParentNode = new BTreeNode(order);
        newParentNode->setIsLeaf(false);
        int parentSplitKey = parent->split(newParentNode);

        handleNonRootSplit(parentSplitKey, parent, newParentNode);
    } else {
        parent->write(file, headerBuffer.headerRecordSize, parent->getCurRBN());
    }
}

void BTreeFile::handleMerge(BTreeNode* parent, BTreeNode *leaf) {
    if (parent == nullptr) {
        return;
    }

    int prevRBN = leaf->getPrevRBN();
    int nextRBN = leaf->getNextRBN();
    int curRBN = leaf->getCurRBN();
    BTreeNode * prevLeaf = new BTreeNode(order);
    BTreeNode * nextLeaf = new BTreeNode(order);

    if (prevRBN != 0) {
        prevLeaf->read(file, headerBuffer.headerRecordSize, prevRBN);
    }

    if (nextRBN != 0) {
        nextLeaf->read(file, headerBuffer.headerRecordSize, nextRBN);
    }

    if (prevRBN != 0 && prevLeaf->isUnderFilled()) {
        int largestKey = prevLeaf->getLargestKey();
        parent->removeKeyAndChildren(largestKey, leaf->getCurRBN());
        prevLeaf->merge(leaf);

        // Update metadata
        nextLeaf->setNextRBN(leaf->getNextRBN());
        leaf->setPrevRBN(0);
        leaf->setNextRBN(headerBuffer.rbnAvail);
        headerBuffer.rbnAvail = leaf->getCurRBN();

        // Write old block (now empty)
        leaf->write(file, headerBuffer.headerRecordSize, curRBN);
        // Write merged block
        prevLeaf->write(file, headerBuffer.headerRecordSize, prevRBN);
        // Write parent
        parent->write(file, headerBuffer.headerRecordSize, parent->getCurRBN());

    } else if (nextRBN != 0 && nextLeaf->isUnderFilled()) {
        int largestKey = leaf->getLargestKey();
        parent->removeKeyAndChildren(largestKey, nextLeaf->getCurRBN());
        leaf->merge(nextLeaf);

        // Update metadata
        leaf->setNextRBN(nextLeaf->getNextRBN());
        nextLeaf->setPrevRBN(0);
        nextLeaf->setNextRBN(headerBuffer.rbnAvail);
        headerBuffer.rbnAvail = nextLeaf->getCurRBN();

        // Write merged block
        leaf->write(file, headerBuffer.headerRecordSize, curRBN);
        // Write old block (now empty)
        nextLeaf->write(file, headerBuffer.headerRecordSize, nextRBN);
        // Write parent
        parent->write(file, headerBuffer.headerRecordSize, parent->getCurRBN());
    } else {
        leaf->write(file, headerBuffer.headerRecordSize, curRBN);
        parent->write(file, headerBuffer.headerRecordSize, parent->getCurRBN());
    }

    if (parent->isUnderFilled()) {
        BTreeNode *newParent = findParentNode(parent);
        handleMerge(newParent, parent);
    }
}

BTreeNode* BTreeFile::findParentNode(BTreeNode* childNode) {
    BTreeNode* currentNode = this->root;
    BTreeNode* parentNode = nullptr;

    while (currentNode && !currentNode->getIsLeaf()) {
        int childRBN = currentNode->getNextChild(childNode->getLargestKey());

        if (childRBN == childNode->getCurRBN()) {
            if (parentNode == nullptr) return root;
            else return currentNode;
        }

        parentNode = currentNode;
        currentNode = new BTreeNode(order);
        currentNode->read(file, headerBuffer.headerRecordSize, childRBN);
    }

    return nullptr;
}

BTreeNode* BTreeFile::findLeafNode(int key) {
    BTreeNode * currentNode = this->root;

    while (!currentNode->getIsLeaf()) {
        int childKey = currentNode->getNextChild(key);
        // read node from child key
        BTreeNode * newNode = new BTreeNode(order);
        newNode->read(file, headerBuffer.headerRecordSize, childKey);
        currentNode = newNode;
    }

    return currentNode;
}

void BTreeFile::displayNode(BTreeNode* node, ostream& ostream, int level, const string& prefix) {
    if (node == nullptr) return;

    ostream << prefix;
    if (level > 0) {
        ostream << "|-- ";
    }

    node->print(ostream);

    if (!node->getIsLeaf()) {
        vector<int> children = node->getChildren();
        for (int i = 0; i < children.size(); i++) {
            BTreeNode * childNode = new BTreeNode(order);
            childNode->read(file, headerBuffer.headerRecordSize, children[i]);
            string newPrefix = prefix + (i < children.size() - 1 ? "|   " : "    ");
            displayNode(childNode, ostream, level + 1, newPrefix);
        }
    }
}

bool BTreeFile::flushData() {
    if (file.is_open()) {
        file.flush();
        file.close();
    } else return false;

    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    return file.is_open();
}


