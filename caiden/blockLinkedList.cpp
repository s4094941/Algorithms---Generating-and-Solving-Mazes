#include "blockLinkedList.h"

#include <iostream>


Node::Node() {
    blockLocation.x = 0;
    blockLocation.y = 0;
    blockLocation.z = 0;
    blockID.id = 0;
    blockID.mod = 0;
    next = NULL;
}


Node::Node(mcpp::Coordinate blockLocation, mcpp::BlockType blockID) {
    this -> blockLocation = blockLocation;
    this -> blockID = blockID;
}


void Node::insertNewNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID) {
    Node *newNode = new Node(blockLocation, blockID);
    if (head == NULL) {
        head = newNode;
    } else {
        newNode -> next = this -> head;
        this -> head = newNode;
    }
}


void Node::repairTerrain() {
    mcpp::MinecraftConnection mc;
    Node *temp = head;
    while (temp != nullptr) {
        mc.setBlock(temp->blockLocation, temp->blockID);
        temp = temp->next;
    }
}


class Node {
    Private:
    mcpp::Coordinate loc;
    mcpp::BlockType block;
    Node* next;

    Public:
    Node(mcpp::coordinate, mcpp::BlockType) {
        this->loc = loc;
        this->block = block;
        next = nullPtr;
    }
    node* getNext() {
        return next;
    }
    getloc
    getblock
}

restoreTerrain() {
    while (history.getNext != nullptr) {
        setBlock(curr.getloc, curr.getBlock)
    }
}