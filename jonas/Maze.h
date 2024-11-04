#ifndef MAZE_H
#define MAZE_H
#include "MazeNode.h"
#include <random>

class Maze {
    private:
        int row, col;
        bool testMode;
        // bool isBuilt;
        MazeNode*** maze;
        std::mt19937 gen;

        // Private methods
        void createGrid();  // TODO: Regenerate base grid (set walls/air)

        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();

        // Generate maze based on testMode
        void generateTestMaze();
        void generateRandomMaze();

        MazeNode* getNodeUp(MazeNode*, int);
        MazeNode* getNodeDown(MazeNode*, int);
        MazeNode* getNodeLeft(MazeNode*, int);
        MazeNode* getNodeRight(MazeNode*, int);

        bool checkNodeUp(MazeNode*, int);
        bool checkNodeDown(MazeNode*, int);
        bool checkNodeLeft(MazeNode*, int);
        bool checkNodeRight(MazeNode*, int);

        bool checkWallUp(MazeNode*);
        bool checkWallDown(MazeNode*);
        bool checkWallLeft(MazeNode*);
        bool checkWallRight(MazeNode*);

        void checkEdge(MazeNode*);
        MazeNode* findStartPoint();
        MazeNode* findIsolatedNode();
        MazeNode* correctNodePos(MazeNode*);
        MazeNode* probeDirection(MazeNode*, int, bool&);
        void checkBothDirections(MazeNode*, MazeNode*, int );
        void printAllDirections();

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

        // Destructor
        ~Maze();
        

        void resetAll();
        void floodFill(MazeNode*);
        void floodFillWall(MazeNode*);


        void connectIsolatedNodes();
};



#endif // MAZE_H


// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid

    // Get random location, send to Agent