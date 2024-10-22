#ifndef MAZE_H
#define MAZE_H
#include "MazeNode.h"

class Maze {
    private:
        int row, col;
        MazeNode*** maze;
        void createGrid();
        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();

    public:
        Maze(int, int);
        void generateTestMaze();
        void generateRandomMaze();
        void generateManualMaze();
        void printMaze();
        ~Maze();
};

#endif // MAZE_H


// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid