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

int main(int argc, char* argv[]) {
    MinecraftConnection mc;
    string mainMenuOption = "";
    string generateMenuOption = "";
    string solveMenuOption = "";
    string cmdLineArg = "";
    bool mode = NORMAL_MODE;
    // bool hasBuilt = false;
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
                // Input will be a Maze object later
                // maze.solveManually();
            }
            else if (solveMenuOption == "2") {
                Agent agent(mc.getPlayerPosition());
                agent.rightHandFollow();
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
