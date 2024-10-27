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

void printGenerateRandMazeMenu() {
    cout << endl << "---------- GENERATE RANDOM MAZE ---------" << endl;
    cout << "1) Regular Mode" << endl;
    cout << "2) Enhancement Mode" << endl;
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

#endif // MENUUTILS_H
