#include <iostream>
#include "Maze.h"

int main (void) {
    int row;
    int col;
    std::cout << "Enter rows and columns: ";
    std::cin >> row >> col;
    std::cout << std::endl;

    Maze maze(row, col);

    //maze.generateRandomMaze();
    maze.generateRandomMaze();
    maze.printMaze();

    return EXIT_SUCCESS;
}