#ifndef MAZE_H
#define MAZE_H

#include "MazeNode.h"
#include <mcpp/mcpp.h>
#include <memory>
#include <thread>
#include <stack>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
using mcpp::Coordinate;

class Maze {
    private:
        int row, col;
        bool testMode;
        bool enhancementMode;
        // bool isBuilt;
        MazeNode*** maze;

        // For E1
        std::vector<Coordinate> mazeBlocks;

        // Private methods
        void createGrid();  // TODO: Regenerate base grid (set walls/air)
        void scanTerrain(Coordinate basePoint);

        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();

        // Generate maze based on testMode
        void generateTestMaze();
        void generateRandomMaze();
        void checkUnexploredArea();

        struct blockNode {
            mcpp::Coordinate blockLocation;
            mcpp::BlockType blockID;
            std::unique_ptr<blockNode> next;

            //Constructor before node is initialized
            blockNode (mcpp::Coordinate initializeLocation, mcpp::BlockType initalizeID)
            : blockLocation(initializeLocation), blockID(initalizeID), next(nullptr) {}
        };
        std::unique_ptr<blockNode> newestNode;
        blockNode* currentNode;



        // NEW ================================
        std::mt19937 gen;

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

    public:
        // Constructor
        Maze(int, int, bool, bool, Coordinate);

        // Accessors
        int getRow();
        int getCol();
        bool getMode();

        // Generate/Build/Print maze
        void createMaze();
        void buildMaze();
        void printMaze();

        // Create Maze (Flatten, Place, Restore)
        void flattenTerrain(mcpp::Coordinate basepoint);
        void placeMaze(mcpp::Coordinate basePoint);
        void restoreTerrain(mcpp::Coordinate basePoint);

        // Constructor
        Maze() : newestNode(nullptr), currentNode(nullptr) {}
        void addNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID); 
        blockNode* getNext(); 

        // void placeMaze();
        void solveManually(Coordinate* basePoint);


        // NEW ================================
        void resetAll();
        void floodFill(MazeNode*);
        void connectIsolatedNodes();
        void breakLoops();
        void validateMaze();

        // Destructor
        ~Maze();
};

#endif // MAZE_H

// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid

    // Get random location, send to Agent
