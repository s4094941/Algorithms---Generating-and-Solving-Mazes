#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
using std::cin;
using std::string;
using std::exception;
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

int main(int argc, char* argv[]) {
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

    if (cmdLineArg == "-testmode") {
        mode = TESTING_MODE;
    }

    try {
        MinecraftConnection mc;
        mc.doCommand("time set day");
        printStartText();
        curState = ST_Main;
        // State machine for menu
        while ((curState != ST_Exit) && !cin.eof()) {
            if (curState == ST_Main) {
                printMainMenu();
                cin >> mainMenuOption;
            }
            if (mainMenuOption == "1") {
                curState = ST_GetMaze;
            }
            else if (mainMenuOption == "2") {
                if (hasGenerated) {
                    if (hasBuilt == false) {
                        cout << "Flattening Terrain";
                        maze->flattenTerrain(*basePoint);
                        cout << "Placing Maze" << endl;
                        maze->placeMaze(*basePoint);
                        cout << "Building Complete";
                        hasBuilt = true;
                    } else if (hasBuilt == true) {
                        cout << "Rebuilding Maze";
                        maze->restoreTerrain(*basePoint);
                        maze->flattenTerrain(*basePoint);
                        maze->placeMaze(*basePoint);
                    }
                }
                else {
                    printNotGeneratedMessage();
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
            else if (!cin.eof()) {
                printInputError1To5Message();
            }

            if (curState == ST_GetMaze) {
                printGenerateMazeMenu();
                cin >> generateMenuOption;
                if (generateMenuOption == "1") {
                    if (hasBuilt == true) {
                        cout << "Fixing Terrain\n";
                        maze->restoreTerrain(*basePoint);
                        hasBuilt = false;
                    }
                    printNavigateMessage();
                    cin >> doneStr;
                    if (doneStr == "done") {
                        printLengthAndWidthMessage();
                        cin >> length >> width;
                        printMazeStructureMessage();
                        // read maze structure here
                        // cin >> 
                        printMazeReadMessage();
                        printStartMazeMessage();
                        cout << "BasePoint: (" << basePoint->x << ", " << 
                            basePoint->y << ", " << basePoint->z << ")" << 
                                endl;
                        printStructureMessage();

                        maze->buildMaze();
                        
                        printEndMazeMessage();
                        
                        if (basePoint != nullptr) {
                            delete basePoint;
                        }
                        basePoint = new Coordinate(mc.getPlayerPosition());

                        if (maze != nullptr) {
                            delete maze;
                        }
                        maze = new Maze(length, width, mode);
                        hasGenerated = true;
                        curState = ST_Main;
                    }
                    else {
                        printInputErrorDoneMessage();
                    }
                    curState = ST_Main;
                }
                else if (generateMenuOption == "2") {
                    if (hasBuilt == true) {
                        cout << "Fixing Terrain\n";
                        maze->restoreTerrain(*basePoint);
                        hasBuilt = false;
                    }
                    printNavigateMessage();
                    cin >> doneStr;
                    if (doneStr == "done") {
                        printLengthAndWidthMessage();
                        cin >> length >> width;
                        
                        if (basePoint != nullptr) {
                            delete basePoint;
                        }
                        basePoint = new Coordinate(mc.getPlayerPosition());

                        if (maze != nullptr) {
                            delete maze;
                        }
                        maze = new Maze(length, width, mode);
                        printMazeGeneratedMessage();
                        printStartMazeMessage();
                        cout << "BasePoint: (" << basePoint->x << ", " << 
                            basePoint->y << ", " << basePoint->z << ")" << 
                                endl;
                        printMazeStructureMessage();
                        maze->createMaze();
                        printEndMazeMessage();
                        hasGenerated = true;
                        curState = ST_Main;
                    }
                    else {
                        printInputErrorDoneMessage();
                    }
                }
                else if (generateMenuOption == "3") {
                    curState = ST_Main;
                }
                else if (!cin.eof()) {
                    printInputError1To3Message();
                }
            }
            else if (curState == ST_SolveMaze) {
                printSolveMazeMenu();
                cin >> solveMenuOption;
                if (solveMenuOption == "1") {
                    if (hasGenerated && hasBuilt) {
                        printMazeReadyMessage();
                        maze->solveManually(basePoint);
                    }
                    else {
                        printNotBuiltMessage();
                    }
                }
                else if (solveMenuOption == "2") {
                    if (hasGenerated && hasBuilt) {
                        Agent agent(mc.getPlayerPosition());
                        agent.rightHandFollow(mode);
                    }
                    else {
                        printNotBuiltMessage();
                    }
                }
                else if (solveMenuOption == "3") {
                    curState = ST_Main;
                }
                else if (!cin.eof()) {
                    printInputError1To3Message();
                }
            }
            else if (curState == ST_Creators) {
                printTeamInfo();
                curState = ST_Main;
            }
            else if (curState == ST_Exit) {
                if (hasGenerated && hasBuilt) {
                    maze->restoreTerrain(*basePoint);
                }
                printExitMessage();
            }
        }
    }
    catch (exception& excpt) {
        cout << excpt.what() << endl;
    }

    if (basePoint != nullptr) {
        delete basePoint;
    }

    if (maze != nullptr) {
        delete maze;
    }

    return EXIT_SUCCESS;
}
