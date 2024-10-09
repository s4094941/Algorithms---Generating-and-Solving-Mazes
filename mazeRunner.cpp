#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
using mcpp::MinecraftConnection;
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

MinecraftConnection mc;

int main() {
    int mainMenuOption = 0;
    int generateMenuOption = 0;
    int solveMenuOption = 0;
    States curState = ST_Main;

    // bool mode = NORMAL_MODE;
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

    return EXIT_SUCCESS;
}
