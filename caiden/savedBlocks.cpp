#include "savedBlocks.h"

// Constructor
storeBlocks::storeBlocks() {
    for (int i = 0; i < 50; ++i) {
        savedCoordinates[i] = mcpp::Coordinate();
        savedBlock[i] = mcpp::BlockType();
    }
}


// Store blocks that are about to be removed
void storeBlocks::saveBlocks(mcpp::Coordinate blockCoordinates, mcpp::BlockType removedBlock, int blockNumber) {
        savedCoordinates[blockNumber] = blockCoordinates;
        savedBlock[blockNumber] = removedBlock;
}

// Adjust the variables given to the current block
void storeBlocks::getBlocks(mcpp::Coordinate* fixWorld, mcpp::BlockType* fixBlocks, int blockNumber) {
    *fixWorld = savedCoordinates[blockNumber];
    *fixBlocks = savedBlock[blockNumber];
}