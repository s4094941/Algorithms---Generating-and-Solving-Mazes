#ifndef MAZE_H
#define MAZE_H

#include "alex/MazeNode.h"
#include <mcpp/mcpp.h>
using mcpp::Coordinate;

class Maze {
    private:
        int row, col;
        bool testMode;
        bool enhancementMode;
        // bool isBuilt;
        MazeNode*** maze;

        // Private methods
        void createGrid();  // TODO: Regenerate base grid (set walls/air)
        void scanTerrain(Coordinate* basePoint);

        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();

        // Generate maze based on testMode
        void generateTestMaze();
        void generateRandomMaze();

    public:
        // Constructor
        Maze(int, int, bool, bool, Coordinate*);

        // Accessors
        int getRow();
        int getCol();
        bool getMode();

        // Generate/Build/Print maze
        void createMaze();
        void buildMaze();
        void printMaze();
        // void placeMaze();
        void solveManually(Coordinate* basePoint);

        // Destructor
        ~Maze();
};

#endif // MAZE_H

// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid

    // Get random location, send to Agent
