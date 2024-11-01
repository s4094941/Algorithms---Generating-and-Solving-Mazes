#ifndef MAZE_H
#define MAZE_H

#include "MazeNode.h"
#include <mcpp/mcpp.h>
using mcpp::Coordinate;
using mcpp::BlockType;

class Maze {
    private:
        int row, col;
        bool testMode;
        bool enhancementMode;
        // bool isBuilt;
        MazeNode*** maze;

        // Private methods
          // TODO: Regenerate base grid (set walls/air)
        void scanTerrain(Coordinate basePoint);

        MazeNode* checkDirection(MazeNode*, int);
        MazeNode* getRandomStart();

        // Generate maze based on testMode
        void generateTestMaze();

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
        

    public:
        // Constructor
        Maze(int, int, bool, bool, Coordinate);
        void createGrid();
        void generateRandomMaze();
        void checkUnexploredArea();

        void placeMaze(Coordinate basePoint);
        void restoreTerrain(Coordinate basePoint);

        Maze() : newestNode(nullptr), currentNode(nullptr) {}
        void addNode(Coordinate blockLocation, BlockType blockID);
        blockNode* getNext();
        

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
