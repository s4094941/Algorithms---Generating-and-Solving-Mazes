#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
using mcpp::MinecraftConnection;
using std::cin;
using std::cout;
using std::endl;
using std::string;

#define NORMAL_MODE 0
#define TESTING_MODE 1

enum States {
    ST_Main,
    ST_GetMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

MinecraftConnection mc;

void solveManually(char** maze, int xLen, int zLen, bool mode);

int main(int argc, char* argv[]) {
    string mainMenuOption = "";
    string generateMenuOption = "";
    string solveMenuOption = "";
    string cmdLineArg = "";
    bool mode = NORMAL_MODE;
    States curState = ST_Main;

    // read Mode
    if (argc == 2) {
        cmdLineArg = argv[1];
    }

    if ((argc == 2) && (cmdLineArg == "-testmode")) {
        mode = TESTING_MODE;
    }
    
    if (mode == TESTING_MODE) {
        // do something later with the testing mode
    }

    mc.doCommand("time set day");

    printStartText();
    curState = ST_Main;
    // State machine for menu
    while (curState != ST_Exit) {
        if (curState == ST_Main) {
            printMainMenu();
            cin >> mainMenuOption;
        }
        if (mainMenuOption == "1") {
            curState = ST_GetMaze;
        }
        else if (mainMenuOption == "2") {
            curState = ST_Main;
        }
        else if (mainMenuOption == "3") {
            curState = ST_SolveMaze;
        }
        else if (mainMenuOption == "4") {
            curState = ST_Creators;
        }
        else if (mainMenuOption == "5") {
            curState = ST_Exit;
        }
        else {
            cout << "Input Error: Enter a number between 1 and 5 ..." << endl;
        }

        if (curState == ST_GetMaze) {
            printGenerateMazeMenu();
            cin >> generateMenuOption;
            if (generateMenuOption == "1") {
                curState = ST_Main;
            }
            else if (generateMenuOption == "2") {
                curState = ST_Main;
            }
            else if (generateMenuOption == "3") {
                curState = ST_Main;
            }
            else {
                cout << "Input Error: Enter a number between 1 and 3 ..." << endl;
            }
        }
        else if (curState == ST_SolveMaze) {
            printSolveMazeMenu();
            cin >> solveMenuOption;
            if (solveMenuOption == "1") {
                curState = ST_Main;
            }
            else if (solveMenuOption == "2") {
                curState = ST_Main;
            }
            else if (solveMenuOption == "3") {
                curState = ST_Main;
            }
            else {
                cout << "Input Error: Enter a number between 1 and 3 ..." << endl;
            }
        }
        else if (curState == ST_Creators) {
            printTeamInfo();
            curState = ST_Main;
        }
        else if (curState == ST_Exit) {
            printExitMassage();
        }
    }

    return EXIT_SUCCESS;
}

void solveManually(char** maze, int xLen, int zLen, bool mode) {
    int airCounter = 0;
    int randAir = 0;
    Coordinate airLoc(0, 0, 0);

    for (int i = 0; i < xLen; i++) {
        for (int j = 0; j < zLen; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }

    if (mode == TESTING_MODE) {
        // airLoc.x = ;
        // airLoc.z = ;
    }
    else {
        srand(time(0));

        for (int i = 0; i < xLen; i++) {
            for (int j = 0; j < zLen; j++) {
                if (maze[i][j] == '.') {
                    airCounter++;
                }
            }
        }

        randAir = rand() % airCounter;
        airCounter = 0;
        for (int i = 0; i < xLen; i++) {
            for (int j = 0; j < zLen; j++) {
                if (maze[i][j] == '.') {
                    airCounter++;
                }

                if (airCounter == randAir) {
                    airLoc.x = i;
                    airLoc.z = j;
                }
            }
        }
    }
    airLoc.y = mc.getHeight(airLoc.x, airLoc.z) + 1;
    mc.setPlayerTilePosition(airLoc);
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
