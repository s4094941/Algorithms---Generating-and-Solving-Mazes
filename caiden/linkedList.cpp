#include "linkedList.h"



void linkedList::addNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID) {
    auto newNode = std::make_unique<Node>(blockLocation, blockID);

    // If there are nodes in the list, link the new node to the current newestNode
    if (newestNode) {
        newNode->next = std::move(newestNode);
    }

    // Move the new node to newestNode
    newestNode = std::move(newNode);

    // Reset currentNode to the new newestNode
    currentNode = newestNode.get();
}



linkedList::Node* linkedList::getNext() {
    if (!currentNode) {
        return nullptr; // No current node, return null
    }
    // Save the current node to return
    Node* placeNode = currentNode; 

    // Advance to the next node
    currentNode = currentNode->next.get(); 

    // Return the saved node
    return placeNode; 
}
