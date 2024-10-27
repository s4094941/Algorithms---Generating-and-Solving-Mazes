#ifndef BLOCK_NODE
#define BLOCK_NODE

#include <memory>
#include <mcpp/mcpp.h>


class linkedList {
    private:
        struct Node {
            mcpp::Coordinate blockLocation;
            mcpp::BlockType blockID;
            std::unique_ptr<Node> next;

            //Constructor before node is initialized
            Node (mcpp::Coordinate initializeLocation, mcpp::BlockType initalizeID)
            : blockLocation(initializeLocation), blockID(initalizeID), next(nullptr) {}
        };
        std::unique_ptr<Node> newestNode;
        Node* currentNode;


    public:
        // Constructor
        linkedList() : newestNode(nullptr), currentNode(nullptr) {}

        void addNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID); 
        Node* getNext(); 
};


#endif