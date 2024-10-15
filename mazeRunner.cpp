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

    if (mode == TESTING_MODE) {
        // airLoc.x = ;
        // airLoc.z = ;
    }
    else {
        for (int i = 0; i < xLen; i++) {
            for (int j = 0; j < zLen; j++) {
                if (maze[i][j] == '.') {
                    airCounter++;
                }
            }
        }

        srand(time(0));
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
