#ifndef SAVED_BLOCKS
#define SAVED_BLOCKS

#include <mcpp/mcpp.h>


class storeBlocks {

    private:
        mcpp::Coordinate savedCoordinates[50];
        mcpp::BlockType savedBlock[50];

    public:
        storeBlocks();
        void saveBlocks(mcpp::Coordinate blockCoordinates, mcpp::BlockType removedBlock, int blockNumber);
        void getBlocks(mcpp::Coordinate* fixWorld, mcpp::BlockType* fixBlocks, int blockNumber);

};



// class savedBlocks {
// 
//     private:
//         mcpp::Coordinate savedCoordinates;
//         mcpp::BlockType savedBlockType;
// 
//     public:
//         void saveBlocks(mcpp::Coordinate blockCoordinates, mcpp::BlockType removedBlock);
//         void getBlocks(mcpp::Coordinate fixWorld, mcpp::BlockType fixBlocks);
// 
// 
// };

#endif