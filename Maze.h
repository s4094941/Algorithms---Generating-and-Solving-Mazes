#ifndef MAZE_H
#define MAZE_H

#include "MazeNode.h"
#include <mcpp/mcpp.h>
<<<<<<< HEAD
#include <memory>
=======
using mcpp::Coordinate;
>>>>>>> 5152ed46ac232279a44bc5cfaab4c5c007f31066

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
        Maze(int, int, bool, mcpp::Coordinate);

        // Accessors
        int getRow();
        int getCol();
        bool getMode();

        // Generate/Build/Print maze
        void createMaze();
        void buildMaze();
        void printMaze();
<<<<<<< HEAD

        // Create Maze (Flatten, Place, Restore)
        mcpp::Coordinate flattenTerrain(int row, int col);
        void placeMaze(int row, int col, mcpp::Coordinate basePoint);
        void restoreTerrain(int row, int col, mcpp::Coordinate basePoint);

        // Constructor
        Maze() : newestNode(nullptr), currentNode(nullptr) {}
        void addNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID); 
        blockNode* getNext(); 


=======
        // void placeMaze();
        void solveManually(Coordinate* basePoint);
>>>>>>> 5152ed46ac232279a44bc5cfaab4c5c007f31066

        // Destructor
        ~Maze();
};

#endif // MAZE_H

// TODO: Check if user-generated maze if valid:
    // check edge of maze. If open areas != 1, invalid
    // if not perfect maze (i.e. existence of a loop), invalid

    // Get random location, send to Agent
