/**
 * @file BTreeNode.cpp
 * @brief Implementation file for the BTreeNode class.
 */

#include "BTreeNode.h"
#include <iostream>
#include <algorithm>

using namespace std;

BTreeNode::BTreeNode(int maxKeys) : maxKeys(maxKeys), minKeys(maxKeys / 2) {
    isLeaf = true;
}

int BTreeNode::read(std::istream& stream, int headerRecordSize, int RBN) {
    int addr = bTreeIndexBuffer.read(stream, headerRecordSize, RBN);
    if (addr != -1) {
        curRBN = addr / headerRecordSize;
        bTreeIndexBuffer.unpack(keys, children);
        numKeys = keys.size();
        isLeaf = false;
    } else {
        return blockBuffer.read(stream, headerRecordSize, RBN);
    }

    return addr;
}

int BTreeNode::write(std::ostream& stream, int headerRecordSize, int RBN) {
    if (isLeaf) {
        return blockBuffer.write(stream, headerRecordSize, RBN);
    } else {
        bTreeIndexBuffer.clear();
        bTreeIndexBuffer.pack(keys, children);
        int addr = bTreeIndexBuffer.write(stream, headerRecordSize, RBN);
        curRBN = addr / headerRecordSize;
        return addr;
    }
}

int BTreeNode::insertRecord(RecordBuffer& recordBuffer) {
    if (!isLeaf || blockBuffer.pack(recordBuffer) == -1 || blockBuffer.sortBuffer() == -1 || blockBuffer.isOverFilled()) {
        return -1;
    }
    return 0;
}

int BTreeNode::removeRecord(RecordBuffer& recordBuffer) {
    if (!isLeaf || blockBuffer.removeRecord(recordBuffer.getRecordKey()) == -1 || blockBuffer.isUnderFilled()) {
        return -1;
    }
    return 0;
}

int BTreeNode::retrieveRecord(RecordBuffer& recordBuffer, int key) {
    while(blockBuffer.unpack(recordBuffer) != -1) {
        if (recordBuffer.getRecordKey() == key) {
            return 0;
        }
    }

    recordBuffer.clear();
    return -1;
}

int BTreeNode::insertKeyAndChildren(int key, int child1, int child2) {
    // Find the position to insert the key
    auto keyIt = std::lower_bound(keys.begin(), keys.end(), key);
    int keyIndex = std::distance(keys.begin(), keyIt);

    // Insert the key
    keys.insert(keyIt, key);

    // When inserting a new root, the children are the original root and the new node created from a split.
    // child1 should be at the current index (as children are one more than keys) and child2 right after it.
    if (child2 != -1) {
        children.insert(children.begin() + keyIndex, child1);
        children.insert(children.begin() + keyIndex + 1, child2);
    } else {
        // For other cases, insert the child to the right of the key
        children.insert(children.begin() + keyIndex + 1, child1);
    }

    numKeys++;
    return 0;
}



int BTreeNode::removeKeyAndChildren(int key, int child) {
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it == keys.end()) {
        return -1;
    }

    keys.erase(it);
    numKeys--;

    it = std::find(children.begin(), children.end(), child);
    if (it == children.end()) {
        return -1;
    }

    children.erase(it);
    return 0;
}

void BTreeNode::print(std::ostream &stream) {
    if (isLeaf) {
        cout << "LEAF NODE: LARGEST KEY = " << getLargestKey() << endl;
    } else {
        cout << "INDEX NODE: RBN = " << getCurRBN() << ", KEYS = ";
        for (int key : keys) {
            stream << key << " ";
        }
        stream << endl;
    }
}

int BTreeNode::split(BTreeNode *newNode) {
    if (isLeaf) {
        blockBuffer.splitBuffer(newNode->blockBuffer);
    } else {
        int midIndex = keys.size() / 2;
        int splitKey = keys[midIndex];  // The key to move up to the parent

        // Move the right half of the keys to the new node
        for (int i = midIndex + 1; i < keys.size(); i++) {
            newNode->keys.push_back(keys[i]);
            newNode->children.push_back(children[i]);
        }
        newNode->children.push_back(children[keys.size()]);

        // Adjust the original node
        keys.erase(keys.begin() + midIndex + 1, keys.end());
        children.erase(children.begin() + midIndex + 1, children.end());

        return splitKey;
    }

    return -1;
}

int BTreeNode::merge(BTreeNode *fromNode) {
    if (isLeaf) {
        blockBuffer.mergeBuffer(fromNode->blockBuffer);
    } else {
        for (auto key : fromNode->keys) {
            keys.push_back(key);
        }
        for (auto child : fromNode->children) {
            keys.push_back(child);
        }
    }

    return 0;
}

bool BTreeNode::getIsLeaf() {
    return isLeaf;
}

void BTreeNode::setIsLeaf(bool isL) {
    isLeaf = isL;
}

int BTreeNode::getNextChild(int key) {
    if (isLeaf) {
        return -1;
    }

    int numChildren = children.size();
    for (int i = 0; i < numChildren - 1; i++) {
        if (key <= keys[i]) {
            return children[i];
        }
    }

    return children[numChildren - 1];
}

std::vector<int> BTreeNode::getChildren() {
    return children;
}

int BTreeNode::getLargestKey() {
    if (isLeaf) {
        return blockBuffer.getLargestKey();
    }

    if (keys.empty()) {
        return -1;
    }

    return keys[keys.size() -1];
}


void BTreeNode::setCurRBN(int rbn) {
    if (isLeaf) {
        blockBuffer.setCurRBN(rbn);
    } else {
        curRBN = rbn;
    }
}

int BTreeNode::getCurRBN() {
    if (isLeaf) {
        return blockBuffer.getCurRBN();
    }

    return curRBN;
}

int BTreeNode::getPrevRBN() {
    if (isLeaf) {
        return blockBuffer.getPrevRBN();
    } else return -1;
}

void BTreeNode::setPrevRBN(int rbn) {
    if (isLeaf) {
        blockBuffer.setPrevRBN(rbn);
    }
}

int BTreeNode::getNextRBN() {
    if (isLeaf) {
        return blockBuffer.getNextRBN();
    } else return -1;
}

void BTreeNode::setNextRBN(int rbn) {
    if (isLeaf) {
        blockBuffer.setNextRBN(rbn);
    }
}

bool BTreeNode::isOverFilled() {
    return numKeys > maxKeys;
}

bool BTreeNode::isUnderFilled() {
    if (isLeaf) {
        return blockBuffer.isUnderFilled();
    } else {
        return numKeys < minKeys;
    }
}
