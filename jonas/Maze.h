// MazeNode*** maze = new MazeNode**[row];
//     for (int i = 0; i < row; ++i) {
//         maze[i] = new MazeNode*[col];
//         for (int j = 0; j < col; ++j) {
//             maze[i][j] = new MazeNode(i, j);
//         }
//     }


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


// Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid