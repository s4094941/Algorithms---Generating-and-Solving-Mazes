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
        void createGrid();

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

        void checkEdge(MazeNode*);
        MazeNode* findStartPoint();
        MazeNode* findIsolatedNode(bool);
        MazeNode* correctNodePos(MazeNode*);
        MazeNode* probePath(MazeNode*, bool&);
        MazeNode* probeWall(MazeNode*, bool&);
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
        void connectIsolatedNodes();
        void breakLoops();
        void validateMaze();
};



#endif // MAZE_H