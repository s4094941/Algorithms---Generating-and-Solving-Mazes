#ifndef MAZE_H
#define MAZE_H
#include "MazeNode.h"

class Maze {
    private:
        MazeNode*** maze;

    public:
        Maze(int, int);
        // Maze(char**);
        void printMaze();
        ~Maze();
};

#endif // MAZE_H


// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid