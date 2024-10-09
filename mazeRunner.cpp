#include <iostream>
#include <mcpp/mcpp.h>
#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
using std::cin;

#define NORMAL_MODE 0
#define TESTING_MODE 1

enum States{
    ST_Main,
    ST_GetMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

mcpp::MinecraftConnection mc;

int main() {
    int menuOption = 0;

    // bool mode = NORMAL_MODE;
    // read Mode
    
    mc.doCommand("time set day"); 

    States curState = ST_Main;

    printStartText();
    // State machine for menu
    while (curState != ST_Exit) {
        printMainMenu();
        cin >> menuOption;
        if (menuOption == 1) {
            printGenerateMazeMenu();
        }
        else if (menuOption == 2) {
            
        }
    }

    printExitMassage();


    return EXIT_SUCCESS;

}