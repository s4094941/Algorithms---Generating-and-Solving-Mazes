#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
using std::cin;
using std::cout;
using std::endl;

#define NORMAL_MODE 0
#define TESTING_MODE 1

enum States {
    ST_Main,
    ST_GetMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

mcpp::MinecraftConnection mc;


void buildMaze(int xLength, int zWidth, char** mazeArray, bool mode) {

    // Variables for Maze Generation
    mcpp::Coordinate playerPos;
    mcpp::Coordinate removeBlock;
    mcpp::Coordinate addBlock;
    mcpp::Coordinate placeWall;    

    // Blocks for walls, removal, and creating the base
    mcpp::BlockType const ACACIA_WOOD_PLANKS(5,4);
    mcpp::BlockType const AIR(0);
    mcpp::BlockType const BEDROCK(7);

    // Store Removed Blocks
    mcpp::Coordinate storeCoordinates[50];
    mcpp::BlockType storeBlockID[50];
    int blockNumber = 0;

    // Get player coordinates
    playerPos = mc.getPlayerPosition();

    // Remove 1y from addBlock to place below player y
    addBlock.y = playerPos.y - 1;

    // Set for first  block being placed
    placeWall.y = playerPos.y;



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
        for (int j = playerPos.z; j < playerPos.z + zWidth; ++j) {
            removeBlock.z = j;
            addBlock.z = j;

            // If statement for too high (Repeat until y coordinate = player)
            if (mc.getHeight(i, j) > playerPos.y - 1) {

                while (mc.getHeight(i, j) > playerPos.y - 1) {
                    removeBlock.y = mc.getHeight(i, j);

                    storeCoordinates[blockNumber] = removeBlock;
                    storeBlockID[blockNumber] = mc.getBlock(removeBlock);

                    mc.setBlock(removeBlock, AIR);
                    blockNumber++;
                }
            }


            // If statement for too low
            if (mc.getHeight(i, j) < playerPos.y - 1) {
                addBlock.y = mc.getHeight(i, j) + 1;
                
                
                while (mc.getHeight(i,j) < playerPos.y - 1) {
                    mc.setBlock(addBlock, BEDROCK);
                    addBlock.y = mc.getHeight(i, j) + 1;
                }
                
            }
        }
    }



    /*
     * Begin Building Maze
     * If alphabet (x), place
     * If symbol (.), do nothing
     * [Test cases for other character done by generation?]
    */

   // Begin loops (i = x, j = z) : Go through array(?) putting ACACIA_WOOD_PLANKS for each (x).
    
    // Current x coordinate
    for (int i = playerPos.x; i < playerPos.x + xLength; ++i) {

        // Current z coordinate
        for (int j = playerPos.z; j < playerPos.z + zWidth; ++j) {

            if (isalpha(mazeArray[i - playerPos.x][j - playerPos.z])) {
                
                // Set placeWall to current detected coordinate
                placeWall.x = i;
                placeWall.z = j;


                // Place a 2 block high wall
                for (int k = 0; k < 2; ++k) {
                    mc.setBlock(placeWall, ACACIA_WOOD_PLANKS);
                    placeWall.y = playerPos.y + 1;
                }
                

                // Reset placeWall.y to player height
                placeWall.y = playerPos.y;
            } else {
                // Space for cases where (.) will need an action
            }

        }
    }



    // Fix Overworld

    // Remove Maze
    // i = Current x coordinate
    for (int i = playerPos.x; i < playerPos.x + xLength; ++i) {
        removeBlock.x = i;
        addBlock.x = i;

        // j = Current z coordinate
        for (int j = playerPos.z; j < playerPos.z + zWidth; ++j) {
            removeBlock.z = j;
            addBlock.z = j;

            // Remove all walls
            if (mc.getHeight(i, j) > playerPos.y - 1) {

                while (mc.getHeight(i, j) > playerPos.y - 1) {
                    removeBlock.y = mc.getHeight(i, j);
                    mc.setBlock(removeBlock, AIR);
                }
            }

            // If block is bedrock, remove
            removeBlock.y = mc.getHeight(i,j);
            while (mc.getBlock(removeBlock) == BEDROCK) {
                mc.setBlock(removeBlock, AIR);
                removeBlock.y = mc.getHeight(i,j);
            }
        }
    }
           
    // Place back deleted blocks
    for (int i = 0; i < blockNumber; ++i) {
        mc.setBlock(storeCoordinates[i], storeBlockID[i]);
    }


}



int main() {
    int mainMenuOption = 0;
    int generateMenuOption = 0;
    int solveMenuOption = 0;
    States curState = ST_Main;
    bool mode = NORMAL_MODE;
    // read Mode
    mc.doCommand("time set day");

    printStartText();
    // State machine for menu
    while (curState != ST_Exit) {
        if (curState == ST_Main) {
            printMainMenu();
        }
        cin >> mainMenuOption;
        if (mainMenuOption == 1) {
            printGenerateMazeMenu();
            curState = ST_GetMaze;
        }
        else if (mainMenuOption == 2) {
            curState = ST_Main;
        }
        else if (mainMenuOption == 3) {
            printSolveMazeMenu();
            curState = ST_SolveMaze;
        }
        else if (mainMenuOption == 4) {
            printTeamInfo();
            curState = ST_Creators;
        }
        else if (mainMenuOption == 5) {
            curState = ST_Exit;
        }
        else {
            cout << "Input Error: Enter a number between 1 and 5 ..." << endl;
            curState = ST_Main;
        }

        if (curState == ST_GetMaze) {
            cin >> generateMenuOption;
            if (generateMenuOption == 1) {

            }
            else if (generateMenuOption == 2) {

            }
            else if (generateMenuOption == 3) {
                curState = ST_Main;
            }
            else {
                cout << "Input Error: Enter a number between 1 and 3 ..." << endl;
                printGenerateMazeMenu();
            }
        }
        else if (curState == ST_SolveMaze) {
            cin >> solveMenuOption;
            if (solveMenuOption == 1) {

            }
            else if (solveMenuOption == 2) {

            }
            else if (solveMenuOption == 3) {
                curState = ST_Main;
            }
            else {
                cout << "Input Error: Enter a number between 1 and 3 ..." << endl;
                printSolveMazeMenu();
            }
        }
        else if (curState == ST_Creators) {
            curState = ST_Main;
        }
    }
    printExitMassage();



    int row = 10;
    int col = 10;

    // Create dynamic array with size [row] * [col]
    char** structure = new char*[row];

    for (int i = 0; i < row; ++i) {
        structure[i] = new char[col];
    }

    // Fill structure (SQUARE)
    for (int i = 0; i < row; ++i) {
        if (i == 0 || i == row - 1) {
            for (int j = 0; j < col; ++j) {
                structure[i][j] = 'x';
            }
        } else {
            for (int j = 0; j < col; ++j) {
                if (j == 0 || j == col - 1) {
                    structure[i][j] = 'x';
                } else {
                    structure[i][j] = '.';
                }
            }
        }
    }

// ============= DEBUG =====================================
    // Print structure
    for (int i = 0; i < row; ++i) {
        //std::cout << "?";
        for (int j = 0; j < col; ++j) {
            //std::cout << "!";
            std::cout << structure[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
// =========================================================

    buildMaze(row, col, structure, mode);

    // Delete structure when done
    for (int i = 0; i < row; ++i) {
        delete[] structure[i];
    }

    delete[] structure;

    


    return EXIT_SUCCESS;

}

