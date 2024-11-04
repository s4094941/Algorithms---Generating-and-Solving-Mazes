#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <stack>
#include <vector>
#include <algorithm>

// Construct maze with x rows and y columns
Maze::Maze(int length, int width, bool testMode) {
    col = length;
    row = width;
    this->testMode = testMode;
    srand(time(0));

    std::random_device rd;
    gen.seed(rd());
    
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

void Maze::setMode(bool mode) {
    testMode = mode;
}

void Maze::createGrid() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            maze[i][j]->setWall(true);
        }
    }
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
}

MazeNode* Maze::getRandomStart() {
    int x = 1;
    int y = 1;
    MazeNode* start;

    // Random odd number between 1 and row - 2
    x = (rand() % ((row - 1) / 2)) * 2 + 1;

    if (x == 1 || x == row - 2) {
        y = (rand() % ((col - 1) / 2)) * 2 + 1;
        // CASE: TOP ROW
        if (x == 1) {
            maze[0][y]->setWall(false);
            maze[0][y]->setExplored(true);
        } else {
        // CASE: BOTTOM ROW
            maze[row - 1][y]->setWall(false);
            maze[row - 1][y]->setExplored(true);
        }
    } else {
        // random between 0 or 1 (start or end)
        y = rand() % 2;
        // CASE: LEFT
        if (y == 0) {
            y = 1;
            maze[x][0]->setWall(false);
            maze[x][0]->setExplored(true);
        } else if (y == 1) {
        // CASE: RIGHT
            y = col - 2;
            maze[x][col - 1]->setWall(false);
            maze[x][col - 1]->setExplored(true);
        }
    }
    start = maze[x][y];
    return start;
}

MazeNode* Maze::checkDirection(MazeNode* curr, int dir) {
    // Row offset and Column offset
    int ros = 0;
    int cos = 0;

    if (dir == 0) {
        ros = -2;
    } else if (dir == 1) {
        ros = 2;
    } else if (dir == 2) {
        cos = -2;
    } else if (dir == 3) {
        cos = 2;
    }

    // Wall row offset and Wall column offset = half of the regular offset
    int wros = ros / 2;
    int wcos = cos / 2;

    MazeNode* next = maze[curr->getRow() + ros][curr->getCol() + cos];
    MazeNode* wall = maze[curr->getRow() + wros][curr->getCol() + wcos];
    if (next->getStatus() == false) {
        next->setPrevNode(curr);
        wall->setExplored(true);
        curr=next;
        curr->setExplored(true);
    }
    return curr;
}

void Maze::createMaze() {
    if (testMode == true) {
        std::cout << "TEST: testmode = " << testMode << std::endl;
        generateTestMaze();
    } else {
        std::cout << "TEST: testmode = " << testMode << std::endl;
        generateRandomMaze();
    }
}

// Generate maze randomly
void Maze::generateRandomMaze() {
    // Set wallStatus to false for all nodes where x and y are odd
    createGrid();

    // Begin randomisation. Begin at random node adjacent to side wall. Check 2 blocks in available directions. If unexplored, new node becomes current. Loop.
    // If no available directions, backtrack and check again. If current node = dummy node, end loop.
    MazeNode* headNode = new MazeNode(-1, -1);
    MazeNode* currNode = getRandomStart();
    currNode->setExplored(true);
    currNode->setPrevNode(headNode);
    
    while (currNode != headNode) {
        if (currNode->getDirCount() == 0) {
            currNode = currNode->getPrevNode();
        } else {
            int dir = currNode->getRandomDirection();
            if (dir == 0) {
                currNode->markUp();
            } else if (dir == 1) {
                currNode->markDown();
            } else if (dir == 2) {
                currNode->markLeft();
            } else if (dir == 3) {
                currNode->markRight();
            }
            currNode = checkDirection(currNode, dir);
        }
    }
}

// Generate maze based on test rules
void Maze::generateTestMaze() {
    // Set wallStatus to false for all nodes where x and y are odd
    createGrid();

    // Test randomisation. Begin at [1][0]. Break available walls in the order:
    // Up, Right, Down, Left. If no walls are available, backtrack until currNode = headNode.
    
    MazeNode* headNode = new MazeNode(-1, -1);
    MazeNode* currNode = maze[1][1];
    currNode->setExplored(true);
    currNode->setPrevNode(headNode);
    maze[1][0]->setWall(false);
    maze[1][0]->setExplored(true);
    
    while (currNode != headNode) {
        if (currNode->getDirCount() == 0) {
            currNode = currNode->getPrevNode();
        } else {
            int dir = currNode->getTestDirection();
            if (dir == 0) {
                currNode->markUp();
            } else if (dir == 1) {
                currNode->markDown();
            } else if (dir == 2) {
                currNode->markLeft();
            } else if (dir == 3) {
                currNode->markRight();
            }
            currNode = checkDirection(currNode, dir);
        }
    }
}

// Generate maze using user input
void Maze::buildMaze() {
    std::cout << "\'x\' for wall, \'.\' for empty space." << std::endl;                             // CHECK IF OUTPUT HAS TO MATCH 100%
    std::cout << "Note: Entering q will fill the remaining structure with walls." << std::endl;     // CHECK IF OUTPUT HAS TO MATCH 100%
    bool quitState = false;
    char currChar = ' ';
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (!quitState) {
                std::cin >> currChar;
                if (currChar == 'x') {
                    maze[i][j]->setWall(true);
                } else if (currChar == '.') {
                    maze[i][j]->setExplored(true);
                } else if (currChar == 'q') {
                    maze[i][j]->setWall(true);
                    std::cout << "filling in remaining structure with walls." << std::endl;
                    quitState = true;
                } else {
                    maze[i][j]->setWall(true);
                    std::cout << "invalid char at [" << i << "][" << j << "]. Defaulted to x" << std::endl;
                }
            } else {
                maze[i][j]->setWall(true);
            }

        }
    }
    printMaze();
    // TODO: CHECK VALIDITY OF USER-GENERATED MAZE
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

int Maze::getRow() {
    return row;
}

int Maze::getCol() {
    return col;
}

bool Maze::getMode() {
    return testMode;
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

// TODO: Remove occurrences of multiple return commands
// TODO: Remove continue, break, goto, next
// TODO: Create checkUp, checkDown, checkLeft, checkRight commands



// NEW =================================================================================
// NEW =================================================================================
// NEW =================================================================================

// Resets all variables to their base values, except row, column, and wall values
void Maze::resetAll() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            maze[i][j]->resetNode();
            checkEdge(maze[i][j]);
        }
    }
}

// Marks invalid directions to check for all nodes in the maze (POTENTIALLY UNNECESSARY WITH NEW ROW/COL CHECKS)
void Maze::checkEdge(MazeNode* node) {
    if (node->getRow() == 0 || node->getRow() == 1) {
        node->markUp();
    }
    if (node->getCol() == 0 || node->getCol() == 1) {
        node->markLeft();
    }
    if (node->getRow() == row - 1 || node->getRow() == row - 2) {
        node->markDown();
    }
    if (node->getCol() == col - 1 || node->getCol() == col - 2) {
        node->markRight();
    }
}

// Finds the first opening in the outer perimeter of the maze. Any additional openings are turned into walls.
MazeNode* Maze::findStartPoint() {
    MazeNode* startPoint = nullptr;

    // CHECK TOP ROW
    for (int i = 1; i < col - 1; ++i) {
        if (maze[0][i]->getWall() == false) {
            if (startPoint == nullptr) {
                startPoint = maze[0][i];
            } else {
                std::cout << "Multiple entry points detected. Node at "
                << "[0][" << i << "]"
                << " converted to a wall." << std::endl;
                maze[0][i]->setWall(true);
            }
        }
    }

    // CHECK BOTTOM ROW
    for (int i = 1; i < col - 1; ++i) {
        if (maze[row - 1][i]->getWall() == false) {
            if (startPoint == nullptr) {
                startPoint = maze[row - 1][i];
            } else {
                std::cout << "Multiple entry points detected. Node at "
                << "[" << row - 1 << "][" << i << "]"
                << " converted to a wall." << std::endl;
                maze[row - 1][i]->setWall(true);
            }
        }
    }

    // CHECK LEFT COLUMN
    for (int i = 1; i < row - 1; ++i) {
        if (maze[i][0]->getWall() == false) {
            if (startPoint == nullptr) {
                startPoint = maze[i][0];
            } else {
                std::cout << "Multiple entry points detected. Node at "
                << "[" << i << "][0]"
                << " converted to a wall." << std::endl;
                maze[i][0]->setWall(true);
            }
        }
    }

    // CHECK RIGHT COLUMN
    for (int i = 0; i < row - 1; ++i) {
        if (maze[i][col - 1]->getWall() == false) {
            if (startPoint == nullptr) {
                startPoint = maze[i][col - 1];
            } else {
                std::cout << "Multiple entry points detected. Node at "
                << "[" << i << "][" << col - 1 << "]"
                << " converted to a wall." << std::endl;
                maze[i][col - 1]->setWall(true);
            }
        }
    }

    return startPoint;
}


// Returns a pointer to the node directly adjacent in a particular direction to the argument provided
MazeNode* Maze::getNodeUp(MazeNode* n, int count) {
    return maze[n->getRow() - count][n->getCol()];
}
MazeNode* Maze::getNodeLeft(MazeNode* n, int count) {
    return maze[n->getRow()][n->getCol() - count];
}

MazeNode* Maze::getNodeDown(MazeNode* n, int count) {
    return maze[n->getRow() + count][n->getCol()];
}

MazeNode* Maze::getNodeRight(MazeNode* n, int count) {
    return maze[n->getRow()][n->getCol() + count];
}

// Check whether the node directly adjacent in a particular direction to the provided argument is an unexplored, non-wall node

bool Maze::checkNodeUp(MazeNode* n, int count) {
    bool nodeAvailable = false;

    if (n->getRow() > 0 + count) {
        if (getNodeUp(n, count)->getWall()   == false
         && getNodeUp(n, count)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}

bool Maze::checkNodeDown(MazeNode* n, int count) {
    bool nodeAvailable = false;
    if (n->getRow() < row - 1 - count) {
        if (getNodeDown(n, count)->getWall() == false
         && getNodeDown(n, count)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeLeft(MazeNode* n, int count) {
    bool nodeAvailable = false;

    if (n->getCol() > 0 + count) {
        if (getNodeLeft(n, count)->getWall()   == false
         && getNodeLeft(n, count)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeRight(MazeNode* n, int count) {
    bool nodeAvailable = false;
    if (n->getCol() < col - 1 - count) {
        if (getNodeRight(n, count)->getWall() == false
         && getNodeRight(n, count)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}

// // Check whether the node directly adjacent in a particular direction to the provided argument is an unexplored, WALL
// bool Maze::checkWallUp(MazeNode* n) {
//     bool nodeAvailable = false;

//     if (n->getRow() > 0) {
//         if (getNodeUp(n)->getWall() == false && getNodeUp(n)->getStatus() == false) {
//             nodeAvailable = true;
//         }
//     }

//     return nodeAvailable;
// }
// bool Maze::checkWallDown(MazeNode* n) {
//     bool nodeAvailable = false;

//     if (n->getRow() < row - 1) {
//         if (getNodeDown(n)->getWall() == false && getNodeDown(n)->getStatus() == false) {
//             nodeAvailable = true;
//         }
//     }

//     return nodeAvailable;
// }
// bool Maze::checkWallLeft(MazeNode* n) {
//     bool nodeAvailable = false;

//     if (n->getCol() > 0) {
//         if (getNodeLeft(n)->getWall() == false && getNodeLeft(n)->getStatus() == false) {
//             nodeAvailable = true;
//         }
//     }

//     return nodeAvailable;
// }
// bool Maze::checkWallRight(MazeNode* n) {
//     bool nodeAvailable = false;

//     if (n->getCol() < row - 1) {
//         if (getNodeRight(n)->getWall() == false && getNodeRight(n)->getStatus() == false) {
//             nodeAvailable = true;
//         }
//     }

//     return nodeAvailable;
// }

// Returns the first instance of a non-wall unexplored node.
MazeNode* Maze::findIsolatedNode() {
    MazeNode* node = nullptr;

    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            if (maze[i][j]->getStatus() == false && maze[i][j]->getWall() == false) {
                node = maze[i][j];
            }
        }
    }

    return node;
}

// Ensures that a node is in an odd x odd location
MazeNode* Maze::correctNodePos(MazeNode* node) {

    // Check if the node is in an even row
    if (node->getRow() % 2 == 0) {
        if (checkNodeUp(node, 1)) {
            node = getNodeUp(node, 1);
        } else if (checkNodeDown(node, 1)) {
            node = getNodeDown(node, 1);
        } else if (node->getRow() > 1) {
            node = getNodeUp(node, 1); // Move up if possible
            node->setWall(false);
        } else if (node->getRow() < row - 2) {
            node = getNodeDown(node, 1); // Move down if possible
            node->setWall(false);
        }
    } 
    // Check if the node is in an even column
    else if (node->getCol() % 2 == 0) {
        if (checkNodeLeft(node, 1)) {
            node = getNodeLeft(node, 1);
        } else if (checkNodeRight(node, 1)) {
            node = getNodeRight(node, 1);
        } else if (node->getCol() > 1) {
            node = getNodeLeft(node, 1); // Move left if possible
            node->setWall(false);
        } else if (node->getCol() < col - 2) {
            node = getNodeRight(node, 1); // Move right if possible
            node->setWall(false);
        }
    }

    std::cout << std::endl;
    return node;
}

// Flood fills using the argument as a starting point. "Marked" nodes are labeled as explored
void Maze::floodFill(MazeNode* startPoint) {
    std::stack<MazeNode*> stack;
   
   stack.push(startPoint);
   startPoint->setExplored(true);

    while (stack.empty() == false) {
        // Set current node to the top of the stack, then pop it off the top of the stack, since it won't need to be checked again.
        // Check nodes in each direction, adding them to the stack and marking them if they are unexplored, non-wall nodes.
        MazeNode* curr = stack.top();
        stack.pop();

        // Check each direciton
        if (checkNodeUp(curr, 1) == true) {
            getNodeUp(curr, 1)->setExplored(true);
            stack.push(getNodeUp(curr, 1));
        }
        if (checkNodeDown(curr, 1) == true) {
            getNodeDown(curr, 1)->setExplored(true);
            stack.push(getNodeDown(curr, 1));
        }
        if (checkNodeLeft(curr, 1) == true) {
            getNodeLeft(curr, 1)->setExplored(true);
            stack.push(getNodeLeft(curr, 1));
        }
        if (checkNodeRight(curr, 1) == true) {
            getNodeRight(curr, 1)->setExplored(true);
            stack.push(getNodeRight(curr, 1));
        }
    }
}

// void Maze::floodFillWall(MazeNode* startPoint) {
//     std::stack<MazeNode*> stack;
//     stack.push(startPoint);
//     startPoint->checkWall();

//     while (stack.empty() == false) {
//         // Set current node to the top of the stack, then pop it off the top of the stack, since it won't need to be checked again.
//         // Check nodes in each direction, adding them to the stack and marking them if they are unexplored, non-wall nodes.
//         MazeNode* curr = stack.top();
//         stack.pop();

//         // Check each direction
//         if (checkWallUp(curr) == true) {
//             getNodeUp(curr)->checkWall();
//             stack.push(getNodeUp(curr));
//         }
//         if (checkWallDown(curr) == true) {
//             getNodeDown(curr)->checkWall();
//             stack.push(getNodeDown(curr));
//         }
//         if (checkWallLeft(curr) == true) {
//             getNodeLeft(curr)->checkWall();
//             stack.push(getNodeLeft(curr));
//         }
//         if (checkWallRight(curr) == true) {
//             getNodeRight(curr)->checkWall();
//             stack.push(getNodeRight(curr));
//         }
//     }
// }


// Randomly joins any isolated nodes to the main path
void Maze::connectIsolatedNodes () {
    resetAll();

    floodFill(findStartPoint());
    MazeNode* isolatedNode = nullptr;
    
    // Initial check to find any isolated nodes
    isolatedNode = findIsolatedNode();

    // Loop until no nodes exist that are isolated from the main path
    while (isolatedNode) {
        isolatedNode = correctNodePos(isolatedNode);
        resetAll();
        floodFill(isolatedNode);

        bool connected = false;
        while (connected == false) {
            if (isolatedNode->getDirCount() > 0) {
                isolatedNode = probeDirection(isolatedNode, connected);
            } else {
                isolatedNode = isolatedNode->getPrevNode();
            }
        }

        resetAll();
        floodFill(findStartPoint());
        isolatedNode = findIsolatedNode();
    }

    resetAll();
    printMaze();
}


MazeNode* Maze::probeDirection(MazeNode* curr, bool& connected) {
    std::vector<int> dirList;
    bool found = false;

    // Add available directions to dirList and shuffle
    if (!curr->getUp()) dirList.push_back(0);
    if (!curr->getDown()) dirList.push_back(1);
    if (!curr->getLeft()) dirList.push_back(2);
    if (!curr->getRight()) dirList.push_back(3);

    // Shuffle available directions to prevent loops
    std::shuffle(dirList.begin(), dirList.end(), gen);

    // 1. Check for unexplored path nodes, if found, end.
    for (int i = 0; i < dirList.size(); ++i) {
        if (!found && dirList[i] == 0) {          // Case: Up
            if (getNodeUp(curr, 2)->getStatus() == false
            && getNodeUp(curr, 2)->getWall() == false) {
                getNodeUp(curr, 1)->setExplored(true);
                connected = true;
                found = true;
            }
        } else if (!found && dirList[i] == 1) {   // Case: Down
            if (getNodeDown(curr, 2)->getStatus() == false
            && getNodeDown(curr, 2)->getWall() == false) {
                getNodeDown(curr, 1)->setExplored(true);
                connected = true;
                found = true;
            }
        } else if (!found && dirList[i] == 2) {   // Case: Left
            if (getNodeLeft(curr, 2)->getStatus() == false
            && getNodeLeft(curr, 2)->getWall() == false) {
                getNodeLeft(curr, 1)->setExplored(true);
                connected = true;
                found = true;
            }
        } else if (!found && dirList[i] == 3) {   // Case: Right
            if (getNodeRight(curr, 2)->getStatus() == false
            && getNodeRight(curr, 2)->getWall() == false) {
                getNodeRight(curr, 1)->setExplored(true);
                connected = true;
                found = true;
            }
        }
    }

    // 2. Break down walls
    if (!found) {
        for (int i = 0; i < dirList.size(); ++i) {
            if (!found && dirList[i] == 0) {        // Case: Up
                if (getNodeUp(curr, 2)->getWall() == true) {
                    getNodeUp(curr, 1)->setExplored(true);
                    getNodeUp(curr, 2)->setExplored(true);
                    getNodeUp(curr, 2)->setPrevNode(curr);
                    curr = getNodeUp(curr, 2);
                    found = true;
                }
            } else if (!found && dirList[i] == 1) { // Case: Down
                if (getNodeDown(curr, 2)->getWall() == true) {
                    getNodeDown(curr, 1)->setExplored(true);
                    getNodeDown(curr, 2)->setExplored(true);
                    getNodeDown(curr, 2)->setPrevNode(curr);
                    curr = getNodeDown(curr, 2);
                    found = true;
                }
            } else if (!found && dirList[i] == 2) { // Case: Left
                if (getNodeLeft(curr, 2)->getWall() == true) {
                    getNodeLeft(curr, 1)->setExplored(true);
                    getNodeLeft(curr, 2)->setExplored(true);
                    getNodeLeft(curr, 2)->setPrevNode(curr);
                    curr = getNodeLeft(curr, 2);
                    found = true;
                }
            } else if (!found && dirList[i] == 3) { // Case: Right
                if (getNodeRight(curr, 2)->getWall() == true) {
                    getNodeRight(curr, 1)->setExplored(true);
                    getNodeRight(curr, 2)->setExplored(true);
                    getNodeRight(curr, 2)->setPrevNode(curr);
                    curr = getNodeRight(curr, 2);
                    found = true;
                }
            }
        }
    }

    // 3. Jump to a possible point in the path
    if (!found) {
        for (int i = 0; i < dirList.size(); ++i) {
            if (!found && dirList[i] == 0) {        // Case: Up
                if (getNodeUp(curr, 2)->getStatus() == true) {
                    curr = getNodeUp(curr, 2);
                    found = true;
                }
            } else if (!found && dirList[i] == 1) { // Case: Down
                if (getNodeDown(curr, 2)->getStatus() == true) {
                    curr = getNodeDown(curr, 2);
                    found = true;
                }
            } else if (!found && dirList[i] == 2) { // Case: Left
                if (getNodeLeft(curr, 2)->getStatus() == true) {
                    curr = getNodeLeft(curr, 2);
                    found = true;
                }
            } else if (!found && dirList[i] == 3) { // Case: Right
                if (getNodeRight(curr, 2)->getStatus() == true) {
                    curr = getNodeRight(curr, 2);
                    found = true;
                }
            }
        }
    }

    if (!found) connected = true;
    return curr;
}


// // checkDirection for connecting isolated nodes. 
// MazeNode* Maze::probeDirection(MazeNode* curr, int dir, bool& connected) {
    
//     int ros, cos;
//     if (dir == 0) ros = -2;  // Up
//     if (dir == 1) ros = 2;   // Down
//     if (dir == 2) cos = -2;  // Left
//     if (dir == 3) cos = 2;   // Right

//     int newRow = curr->getRow() + ros;
//     int newCol = curr->getCol() + cos;

//     // probably unnecessary since the directions are set
//     if (newRow < 0 || newRow >= row - 1 || newCol < 0 || newCol >= col - 1) {
//         return curr->getPrevNode();  // Backtrack if out of bounds
//     }

//     MazeNode* next = maze[newRow][newCol];
//     MazeNode* wall = maze[curr->getRow() + ros / 2][curr->getCol() + cos / 2];

//     // 1. If next node is part of the main path, connect and mark as connected
//     if (!next->getWall() && !next->getStatus()) {
//         wall->setWall(false);       // Remove wall to create a connection
//         connected = true;           // Successfully connected to main path
//     }
//     // 2. If next node is in the isolated area, move forward only if unvisited
//     else if (!next->getWall() && next->getStatus() && !next->getPrevNode()) {
//         checkBothDirections(curr, next, dir);
//         next->setPrevNode(curr);    // Set previous node to avoid cycles
//         curr = next;
//     }
//     // 3. If next is a wall, break through as a last resort
//     else if (next->getWall() && !next->getStatus()) {
//         checkBothDirections(curr, next, dir);
//         wall->setWall(false);       // Break through the wall
//         next->setWall(false);
//         next->setExplored(true);
//         next->setPrevNode(curr);
//         curr = next;
//     }
//     // 4. If no progress is possible, mark as fully visited and backtrack
//     else {
//         curr->setExplored(true);     // Mark as fully visited to avoid revisiting
//         curr = curr->getPrevNode(); // Backtrack to previous node
//     }

//     return curr;
// }



void Maze::checkBothDirections(MazeNode* curr, MazeNode* next, int dir) {
    // CHECK UP
    if (dir == 0) {
        curr->markUp();
        next->markDown();
    //  CHECK DOWN
    } else if (dir == 1) {
        curr->markDown();
        next->markUp();
    // CHECK LEFT
    } else if (dir == 2) {
        curr->markLeft();
        next->markRight();
    // CHECK RIGHT
    } else if (dir == 3) {
        curr->markRight();
        next->markLeft();
    }
}

// //====================================================================================