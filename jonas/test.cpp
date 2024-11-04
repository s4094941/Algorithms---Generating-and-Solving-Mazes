#include <iostream>
#include "Maze.h"

int main (void) {
    int row;
    int col;
    int mode;
    std::cout << "Enter rows and columns: ";
    std::cin >> row >> col;
    std::cout << std::endl;

    Maze maze (row, col, false);

    std::cout << "[1] Generate random maze" << std::endl;
    std::cout << "[2] Generate test maze" << std::endl;
    std::cout << "[3] User-generated maze" << std::endl;
    std::cout << "enter your option" << std::endl;
    std::cin >> mode;

    if (mode == 2) {
        maze.setMode(true);
    }

    if (mode == 1 || mode == 2) {
        maze.createMaze();
        //maze.printMaze();
    } else if (mode == 3) {
        maze.buildMaze();
        maze.connectIsolatedNodes();
        std::cout << std::endl << std::endl;
        maze.printMaze();
    }

    return EXIT_SUCCESS;
}