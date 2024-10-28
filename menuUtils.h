#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <iostream>
using std::cout;
using std::endl;

void printStartText() {
    cout << endl << "Welcome to MineCraft MazeRunner!" << endl;
    cout << "--------------------------------" << endl;
}

void printMainMenu() {
    cout << endl << "------------- MAIN MENU -------------" << endl;
    cout << "1) Generate Maze" << endl;
    cout << "2) Build Maze in MineCraft" << endl;
    cout << "3) Solve Maze" << endl;
    cout << "4) Show Team Information" << endl;
    cout << "5) Exit" << endl;
    cout << endl << "Enter Menu item to continue: " << endl;
}

void printGenerateMazeMenu() {
    cout << endl << "------------- GENERATE MAZE -------------" << endl;
    cout << "1) Read Maze from terminal" << endl;
    cout << "2) Generate Random Maze" << endl;
    cout << "3) Back" << endl;
    cout << endl << "Enter Menu item to continue: " << endl;
}

void printSolveMazeMenu() {
    cout << endl << "------------- SOLVE MAZE -------------" << endl;
    cout << "1) Solve Manually" << endl;
    cout << "2) Show Escape Route" << endl;
    cout << "3) Back" << endl;
    cout << endl << "Enter Menu item to continue: " << endl;
}

void printTeamInfo() {
    cout << endl << "Team members:" << endl;
    cout << "\t [1] Alexander Davidson (s4089452@student.rmit.edu.au)" << endl;
    cout << "\t [2] Caiden da Costa (s4904941@student.rmit.edu.au)" << endl;
    cout << "\t [3] Jonas Inocencio (s3787701@student.rmit.edu.au)" << endl;
    cout << endl;
}

void printExitMessage() {
    cout << endl << "The End!" << endl << endl;
}

void printNotGeneratedMessage() {
    cout << "Cannot build a maze without generating a maze ..." << endl;
}

void printInputError1To5Message() {
    cout << "Input Error: Enter a number between 1 and 5 ..." << endl;
}

void printInputError1To3Message() {
    cout << "Input Error: Enter a number between 1 and 3 ..." << endl;
}

void printNavigateMessage() {
    cout << "In Minecraft, navigate to where you need the maze" << endl << 
        "to be built in Minecraft and type - done:" << endl;
}

void printLengthAndWidthMessage() {
    cout << "Enter the length and width of maze:" << endl;
}

void printMazeStructureMessage() {
    cout << "Enter the maze structure:" << endl;
}

void printMazeReadMessage() {
    cout << "Maze read successfully" << endl;
}

void printStartMazeMessage() {
    cout << "**Printing Maze**" << endl;
}

void printStructureMessage() {
    cout << "Structure:" << endl;
}

void printEndMazeMessage() {
    cout << "**End Printing Maze**" << endl;
}

void printInputErrorDoneMessage() {
    cout << "Type \'done\' exactly" << endl;
}

void printMazeGeneratedMessage() {
    cout << "Maze generated successfully" << endl;
}

void printMazeReadyMessage() {
    cout << "Maze ready to Solve..." << endl;
}

void printNotBuiltMessage() {
    cout << "Cannot place player in a maze without " << "building a maze ..." 
        << endl;
}

#endif // MENUUTILS_H
