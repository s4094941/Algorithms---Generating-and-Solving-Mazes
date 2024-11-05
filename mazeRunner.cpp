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
    string doneStr = "done";
    bool mode = NORMAL_MODE;
    bool enhancementMode = false;
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
        // Add a mc server check if server is running otherwise catch
        MinecraftConnection mc;
        mc.doCommand("time set day");

        // Add a player check to check if player is on server otherwise catch
        mc.getPlayerPosition();

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
            // Build Maze in Minecraft
            else if (mainMenuOption == "2") {
                if (hasGenerated) {
                    if (hasBuilt) {
                        cout << "Rebuilding Maze" << endl;
                        maze->restoreTerrain(*basePoint);
                    }
                    // Flatten terrain for base program
                    if (!enhancementMode) {
                        cout << "Flattening Terrain" << endl;
                        maze->flattenTerrain(*basePoint);
                    }
                    cout << "Placing Maze" << endl;
                    maze->placeMaze(*basePoint);
                    hasBuilt = true;
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
                    enhancementMode = false;
                    if (hasBuilt == true) {
                        cout << "Fixing Terrain\n";
                        maze->restoreTerrain(*basePoint);
                        hasBuilt = false;
                    }
                    if (mode == TESTING_MODE) {
                        mc.setPlayerPosition(Coordinate(4848, 71, 4369));
                    }
                    else {
                        printNavigateMessage();
                        cin >> doneStr;
                        if (doneStr != "done") {
                            printInputErrorDoneMessage();
                        }
                    }

                    if (doneStr == "done") {
                        printLengthAndWidthMessage();
                        cin >> length >> width;

                        if (basePoint != nullptr) {
                            delete basePoint;
                            basePoint = nullptr;
                        }
                        basePoint = new Coordinate(mc.getPlayerPosition());
                        cout << "BasePoint: (" << basePoint->x << ", " << 
                            basePoint->y << ", " << basePoint->z << ")" << 
                            endl;

                        if (maze != nullptr) {
                            delete maze;
                            maze = nullptr;
                        }
                        maze = new Maze(length, width, mode, enhancementMode, *basePoint);

                        printEnterStructureMessage();
                        maze->buildMaze();

                        printMazeReadMessage();
                        printStartMazeMessage();
                        printStructureMessage();
                        
                        maze->resetAll();
                        maze->printMaze();
                        
                        printEndMazeMessage();

                        char valOption;
                        std::cout << "Would you like to validate the maze?\n";
                        std::cout << "This will remove all loops and connect isolated nodes to the main path.\n";
                        std::cout << "[y]: yes\t[n]: no" << std::endl;
                        std::cin >> valOption;

                        if (valOption == 'y' || valOption == 'Y') {
                            maze->validateMaze();
                            std::cout << "\nMaze after validation:" << std::endl;
                            maze->resetAll();
                            maze->printMaze();
                        } else {
                            std::cout << "\nMaze will not be validated." << std::endl;
                        }
                        
                        hasGenerated = true;
                        curState = ST_Main;
                    }
                    curState = ST_Main;
                }
                else if (generateMenuOption == "2") {
                    enhancementMode = false;
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
                            basePoint = nullptr;
                        }
                        basePoint = new Coordinate(mc.getPlayerPosition());

                        if (maze != nullptr) {
                            delete maze;
                            maze = nullptr;
                        }
                        maze = new Maze(length, width, mode, enhancementMode, 
                                        *basePoint);
                        printMazeGeneratedMessage();
                        printStartMazeMessage();
                        cout << "BasePoint: (" << basePoint->x << ", " << 
                            basePoint->y << ", " << basePoint->z << ")" << 
                            endl;
                        printStructureMessage();
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
                    enhancementMode = true;
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
                            basePoint = nullptr;
                        }
                        basePoint = new Coordinate(mc.getPlayerPosition());

                        if (maze != nullptr) {
                            delete maze;
                            maze = nullptr;
                        }
                        maze = new Maze(length, width, mode, enhancementMode, 
                                        *basePoint);
                        printMazeGeneratedMessage();
                        printStartMazeMessage();
                        cout << "BasePoint: (" << basePoint->x << ", " << 
                            basePoint->y << ", " << basePoint->z << ")" << 
                            endl;
                        printStructureMessage();
                        maze->createMaze();
                        printEndMazeMessage();
                        hasGenerated = true;
                        curState = ST_Main;
                    }
                    else {
                        printInputErrorDoneMessage();
                    }
                }
                else if (generateMenuOption == "4") {
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
                        printNotBuiltMessagePlace();
                        curState = ST_Main;
                    }
                }
                else if (solveMenuOption == "2") {
                    if (hasGenerated && hasBuilt) {
                        Agent agent(mc.getPlayerPosition());
                        agent.rightHandFollow(mode);
                    }
                    else {
                        printNotBuiltMessageEscape();
                        curState = ST_Main;
                    }
                } else if (solveMenuOption == "3") {
                    if (hasGenerated && hasBuilt) {
                        Agent agent(mc.getPlayerPosition());
                        cout << "Determining Shortest Path..." << endl;
                        agent.BFS();
                        cout << "Shortest Path Calculated.\nDisplaying Path..." << endl;
                        agent.showShortestPath();
                        cout << "Exit has been located" << endl;
                    } else {
                        printNotBuiltMessageEscape();
                        curState = ST_Main;
                    }
                }
                else if (solveMenuOption == "4") {
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
        basePoint = nullptr;
    }

    if (maze != nullptr) {
        delete maze;
        maze = nullptr;
    }

    return EXIT_SUCCESS;
}
