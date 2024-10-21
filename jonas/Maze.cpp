#include "Maze.h"
#include <iostream>
#include <cstdlib>

// Construct maze with x rows and y columns
Maze::Maze(int x, int y) {
    row = x;
    col = y;

    if (row < 3) {
        row = 3;
         std::cout << "Invalid row value. Defaulting to 3" << std::endl;
    }
    if (col < 3) {
        col = 3;
        std::cout << "Invalid row value. Defaulting to 3" << std::endl;
    }
    if (row % 2 == 0) {
        ++row;
        std::cout << "Row value must be odd. Converted to " << row << std::endl;
    }
    if (col % 2 == 0) {
        ++col;
        std::cout << "Column value must be odd. Converted to " << col << std::endl;
    }

    maze = new MazeNode**[row];
    for (int i = 0; i < row; ++i) {
        maze[i] = new MazeNode*[col];
        for (int j = 0; j < col; ++j) {
            maze[i][j] = new MazeNode(i, j);
        }
    }
}

// Generate maze randomly
void Maze::generateRandomMaze() {
    // Set wallStatus to false for all nodes where x and y are odd
    for (int i = 0; i < row; ++i) {
        if (i % 2 != 0) {
            for (int j = 0; j < col; ++j) {
                if (j % 2 != 0) {
                    // set as unexplored node
                    maze[i][j]->setWall(false);
                    // check top row
                    if (i == 1) {
                        maze[i][j]->markUp();
                    }
                    // check bottom row
                    if (i == row - 2) {
                        maze[i][j]->markDown();
                    }
                    if (j == 1) {
                        maze[i][j]->markLeft();
                    }
                    if (j == col - 2) {
                        maze[i][j]->markRight();
                    }
                }
            }
        }
    }

    // Begin randomisation. Begin at random node in row index 1. Check 2 blocks in available directions. If unexplored, new node becomes current. Loop.
    // If no available directions, backtrack and check again. If current node = dummy node, end loop.
    int startCol = (rand() % ((col - 1) / 2)) * 2 + 1;
    
    MazeNode* headNode = new MazeNode(-1, -1);
    MazeNode* currNode = maze[1][startCol];
    currNode->setExplored(true);
    currNode->setPrevNode(headNode);
    maze[0][startCol]->setWall(false);
    maze[0][startCol]->setExplored(true);
    
    while (currNode != headNode) {
    // bool checkDirection(int dir) --------------------
        if (currNode->getDirCount() == 0) {
            currNode = currNode->getPrevNode();
            continue;
        }
        int dir = currNode->getRandomDirection();
    // check node 2 blocks UP
        if (dir == 0) {
            currNode->markUp();
            MazeNode* nextNode =  maze[currNode->getRow() - 2][currNode->getCol()];
            MazeNode* nextWall = maze[currNode->getRow() - 1][currNode->getCol()];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks DOWN            
        } else if (dir == 1) {
            currNode->markDown();
            MazeNode* nextNode =  maze[currNode->getRow() + 2][currNode->getCol()];
            MazeNode* nextWall = maze[currNode->getRow() + 1][currNode->getCol()];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks LEFT            
        } else if (dir == 2) {
            currNode->markLeft();
            MazeNode* nextNode =  maze[currNode->getRow()][currNode->getCol() - 2];
            MazeNode* nextWall = maze[currNode->getRow()][currNode->getCol() - 1];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks RIGHT
        } else if (dir == 3) {
            currNode->markRight();
            MazeNode* nextNode =  maze[currNode->getRow()][currNode->getCol() + 2];
            MazeNode* nextWall = maze[currNode->getRow()][currNode->getCol() + 1];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
        }
    }
}

// Generate maze based on test rules
void Maze::generateTestMaze() {
    // Set wallStatus to false for all nodes where x and y are odd
    for (int i = 0; i < row; ++i) {
        if (i % 2 != 0) {
            for (int j = 0; j < col; ++j) {
                if (j % 2 != 0) {
                    // set as unexplored node
                    maze[i][j]->setWall(false);
                    // check top row
                    if (i == 1) {
                        maze[i][j]->markUp();
                    }
                    // check bottom row
                    if (i == row - 2) {
                        maze[i][j]->markDown();
                    }
                    if (j == 1) {
                        maze[i][j]->markLeft();
                    }
                    if (j == col - 2) {
                        maze[i][j]->markRight();
                    }
                }
            }
        }
    }

    // Begin randomisation. Begin at random node in row index 1. Check 2 blocks in available directions. If unexplored, new node becomes current. Loop.
    // If no available directions, backtrack and check again. If current node = dummy node, end loop.
    
    MazeNode* headNode = new MazeNode(-1, -1);
    MazeNode* currNode = maze[1][1];
    currNode->setExplored(true);
    currNode->setPrevNode(headNode);
    maze[1][0]->setWall(false);
    maze[1][0]->setExplored(true);
    
    while (currNode != headNode) {
    // bool checkDirection(int dir) --------------------
        if (currNode->getDirCount() == 0) {
            currNode = currNode->getPrevNode();
            continue;
        }
        int dir = currNode->getTestDirection();
    // check node 2 blocks UP
        if (dir == 0) {
            currNode->markUp();
            MazeNode* nextNode =  maze[currNode->getRow() - 2][currNode->getCol()];
            MazeNode* nextWall = maze[currNode->getRow() - 1][currNode->getCol()];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks DOWN            
        } else if (dir == 1) {
            currNode->markDown();
            MazeNode* nextNode =  maze[currNode->getRow() + 2][currNode->getCol()];
            MazeNode* nextWall = maze[currNode->getRow() + 1][currNode->getCol()];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks LEFT            
        } else if (dir == 2) {
            currNode->markLeft();
            MazeNode* nextNode =  maze[currNode->getRow()][currNode->getCol() - 2];
            MazeNode* nextWall = maze[currNode->getRow()][currNode->getCol() - 1];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks RIGHT
        } else if (dir == 3) {
            currNode->markRight();
            MazeNode* nextNode =  maze[currNode->getRow()][currNode->getCol() + 2];
            MazeNode* nextWall = maze[currNode->getRow()][currNode->getCol() + 1];
            if (nextNode->getStatus() == false) {
                nextNode->setPrevNode(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
        }
    }
}

// Generate maze using user input
void Maze::generateManualMaze() {
    std::cout << "TODO: IMPLEMENT FEATURE" << std::endl;
}

// Print all nodes in terminal
void Maze::printMaze() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            maze[i][j]->printNode();
        }
        std::cout << std::endl;
    }
}

// Destructor
Maze::~Maze() {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                delete maze[i][j];
            }
            delete[] maze[i];
        }
        delete[] maze;
}

// ASSUMPTIONS:
    // row and column must both be positive odd integers. If a value below 3 is passed, it will be converted to 3.
        // Any even values will have 1 added to them such that they are odd.


// Random exit
    // random row between 0 and row - 1
        // if rand == 0 or row - 1
            // exit = random between 1 and col - 1
        // else
            // exit = 0 or col - 1

