#include <iostream>

#include <mcpp/mcpp.h>

#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"

#define NORMAL_MODE 0
#define TESTING_MODE 1

enum States{
    ST_Main,
    ST_GetMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

int main(void){

    //bool mode = NORMAL_MODE;
    //read Mode

    

    mcpp::MinecraftConnection mc; 
    mc.doCommand("time set day"); 

    States curState = ST_Main;

    //State machine for menu        
    while (curState != ST_Exit)
    {
        //Do something
    }

    printExitMassage();


    return EXIT_SUCCESS;

}

int buildMaze(int xLength, int zWidth, char** mazeArray) {

    mcpp::MinecraftConnection mc;

    // Variables for Maze Generation
    mcpp::Coordinate playerPos;
    mcpp::Coordinate removeBlock;
    mcpp::Coordinate addBlock;

    // Blocks for walls, removal, and creating the base
    mcpp::BlockType const ACACIA_WOOD_PLANKS(5,4);
    mcpp::BlockType const AIR(0,0);
    mcpp::BlockType const BEDROCK(7.0);

    // Get player coordinates
    playerPos = mc.getPlayerPosition();

    // Remove 1y from addBlock to place below player y
    addBlock.y = addBlock.y - 1;



    /*
     * Create Loop to check every block within the coordinates
     * If y coordinate is too high, store block ID and coordinates
     * If y coordinate is too low, add a block where the players feet are, and store coordinates.
    */

//Scan for highest block in each coordinate

    // i = Current x coordinate
    for (int i = playerPos.x; i < playerPos.x + xLength; ++i) {
        removeBlock.x = i;
        addBlock.x = i;

        // j = Current z coordinate
        for (int j = playerPos.z; j < playerPos.z; ++j) {
            removeBlock.z = j;
            addBlock.z = j;

            // If statement for too high (Repeat until y coordinate = player)
            if (mc.getHeight(i, j) > playerPos.y) {

                while (mc.getHeight(i, j) > playerPos.y) {
                    mc.setBlock(removeBlock, AIR);
                }
            }


            // If statement for too low
            if (mc.getHeight(i, j) < playerPos.y) {
                mc.setBlock(addBlock, BEDROCK);
            }
        }
    }

    



    /*
     * Begin Building Maze
     * If alphabet (x), place
     * If symbol (.), do nothing
     * [Test cases for other character done by generation?]
    */

   // Begin loops (i = x, j = z) : Go through array(?) putting BRICK for each (x).






    return EXIT_SUCCESS;
}