#ifndef BLOCK_LINKED_LIST
#define BLOCK_LINKED_LIST

#include <mcpp/mcpp.h>

class Node {
    private:
        mcpp::Coordinate blockLocation;
        mcpp::BlockType blockID;
        Node *next;

        Node();
        Node(mcpp::Coordinate blockLocation, mcpp::BlockType blockID);

    public:
        void insertNewNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID);
        void repairTerrain();
};

#endif