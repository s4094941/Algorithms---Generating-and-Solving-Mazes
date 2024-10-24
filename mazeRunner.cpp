#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
using std::cin;
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
    string doneStr = "";
    bool mode = NORMAL_MODE;
    bool hasBuilt = false;
    bool hasGenerated = false;
    States curState = ST_Main;
    Maze* maze = nullptr;
    Coordinate* basePoint = nullptr;
    int length = 0;
    int width = 0;

    // read Mode
    if (argc == 2) {
        cmdLineArg = argv[1];
    }

    if ((argc == 2) && (cmdLineArg == "-testmode")) {
        mode = TESTING_MODE;
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
            if (hasGenerated) {
                // Call building maze function
                hasBuilt = true;
            }
            else {
                cout << "Cannot build a maze without generating a maze ..." << 
                    endl;
            }
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
                cout << "In Minecraft, navigate to where you need the maze to" 
                    << "be built in Minecraft and type - done:" << endl;

                if (basePoint != nullptr) {
                    delete basePoint;
                }
                basePoint = new Coordinate(mc.getPlayerPosition());

                if (maze != nullptr) {
                    delete maze;
                }
                // user input to constructor
                maze = new Maze(0, 0, mode);

                hasGenerated = true;
                curState = ST_Main;
            }
            else if (generateMenuOption == "3") {
                curState = ST_Main;
            }
            else {
                cout << "Input Error: Enter a number between 1 and 3 ..." <<
                    endl;
            }
        }
        else if (curState == ST_SolveMaze) {
            printSolveMazeMenu();
            cin >> solveMenuOption;
            if (solveMenuOption == "1") {
                if (hasGenerated && hasBuilt) {
                    maze->solveManually(basePoint);
                }
                else {
                    cout << "Cannot place player in a maze without building a"
                        << "maze ..." << endl;
                }
            }
            else if (solveMenuOption == "2") {
                if (hasGenerated && hasBuilt) {
                    Agent agent(mc.getPlayerPosition());
                    agent.rightHandFollow();
                }
                else {
                    cout << "Cannot solve a maze without building a maze ..." 
                        << endl;
                }
            }
            else if (solveMenuOption == "3") {
                curState = ST_Main;
            }
            else {
                cout << "Input Error: Enter a number between 1 and 3 ..." << 
                    endl;
            }
        }
        else if (curState == ST_Creators) {
            printTeamInfo();
            curState = ST_Main;
        }
        else if (curState == ST_Exit) {
            printExitMessage();
        }
    }

    if (basePoint != nullptr) {
        delete basePoint;
    }

    if (maze != nullptr) {
        delete maze;
    }

    return EXIT_SUCCESS;
}
