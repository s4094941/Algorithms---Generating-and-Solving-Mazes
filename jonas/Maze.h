#ifndef MAZE_H
#define MAZE_H
#include "MazeNode.h"

class Maze {
    private:
        int row, col;
        bool testMode;
        // bool isBuilt;
        MazeNode*** maze;
        void createGrid();
        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();
        void generateTestMaze();
        void generateRandomMaze();

    public:
        Maze(int, int, bool);
        int getRow();
        int getCol();
        bool getMode();
        void createMaze();
        void buildMaze();
        void printMaze();
        // void placeMaze();
        ~Maze();
};

#endif // MAZE_H


// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid