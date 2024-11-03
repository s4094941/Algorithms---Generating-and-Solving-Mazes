#ifndef MAZE_H
#define MAZE_H
#include "MazeNode.h"


class Maze {
    private:
        int row, col;
        bool testMode;
        // bool isBuilt;
        MazeNode*** maze;

        // Private methods
        void createGrid();  // TODO: Regenerate base grid (set walls/air)

        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();

        // Generate maze based on testMode
        void generateTestMaze();
        void generateRandomMaze();

        MazeNode* getNodeUp(MazeNode*);
        MazeNode* getNodeLeft(MazeNode*);
        MazeNode* getNodeDown(MazeNode*);
        MazeNode* getNodeRight(MazeNode*);

        bool checkNodeUp(MazeNode*);
        bool checkNodeLeft(MazeNode*);
        bool checkNodeDown(MazeNode*);
        bool checkNodeRight(MazeNode*);

        void checkEdge(MazeNode*);
        MazeNode* findStartPoint();

    public:
        // Constructor
        Maze(int, int, bool);

        void setMode(bool);

        // Accessors
        int getRow();
        int getCol();
        bool getMode();

        // Generate/Build/Print maze
        void createMaze();
        void buildMaze();
        void printMaze();
        // void placeMaze();

        void resetAll();
        void floodFill(MazeNode*);

        // Destructor
        ~Maze();
};



#endif // MAZE_H


// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid

    // Get random location, send to Agent