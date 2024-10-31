#include <thread>
#include "Maze.h"
using mcpp::MinecraftConnection;
using mcpp::Coordinate;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

// Construct maze with x rows and y columns
Maze::Maze(int x, int y, bool testMode) {
    row = x;
    col = y;
    //this -> playerPos = playerPos;
    this->testMode = testMode;
    srand(time(0));

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

void Maze::createGrid() {
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
    if (testMode) {
        generateTestMaze();
    } else {
        generateRandomMaze();
    }
}

// Generate maze randomly
void Maze::generateRandomMaze() {
    // Set wallStatus to false for all nodes where x and y are odd
    createGrid();

    // Random generation. Begin at random node adjacent to side wall. Check 2 blocks in available directions. If unexplored, new node becomes current. Loop.
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
    printMaze();
}

// Generate maze based on test rules
void Maze::generateTestMaze() {
    // Set wallStatus to false for all nodes where x and y are odd
    createGrid();

    // Test mode generation. Begin at [1][0]. Break available walls in the order:
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
    printMaze();
}

// Generate maze using user input
void Maze::buildMaze() {
    std::cout << "USER GENERATED MAZE:" << std::endl;
    std::cout << "'x' for wall, '.' for empty space.";
    std::cout << "Note: Entering q will fill the remaining structure with walls." << std::endl;
    std::cout << "Construct your [" << row << " x " << col << "] structure:" << std::endl;
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

void Maze::flattenTerrain(mcpp::Coordinate basePoint) {
/*
 * Create Loop to check every block within the coordinates
 * If y coordinate is too high, store block ID and coordinates
 * If y coordinate is too low, add a block where the players feet are, and store coordinates.
*/
    
    mcpp::MinecraftConnection mc;

    // Remove Block and Place Block coordinates
    mcpp::Coordinate removeBlock;
    mcpp::Coordinate addBlock;

    // Remove/Add Block
    mcpp::BlockType const AIR(0);
    mcpp::BlockType tempPlacement(0);

    // i = Current z coordinate
    for (int i = basePoint.z; i < basePoint.z + col; ++i) {
        removeBlock.z = i;
        addBlock.z = i;

        // j = Current x coordinate
        for (int j = basePoint.x; j < basePoint.x + row; ++j) {
            removeBlock.x = j;
            addBlock.x = j;

            // If too high, destroy until reaching player's feet, and add destroyed blocks to linked list
            if (mc.getHeight(j, i) > basePoint.y - 1) {

                while (mc.getHeight(j, i) > basePoint.y - 1) {
                    removeBlock.y = mc.getHeight(j, i);
                    addNode(removeBlock, mc.getBlock(removeBlock));
                    mc.setBlock(removeBlock, AIR);
                }
            }

            // If too low, add a block (using the block being built upon) and add the block to linked list
            if (mc.getHeight(j, i) < basePoint.y - 1) {
                addBlock.y = mc.getHeight(j, i);

                while (mc.getHeight(j, i) < basePoint.y - 1) {
                    tempPlacement = mc.getBlock(addBlock);
                    addBlock.y = mc.getHeight(j, i) + 1;
                    mc.setBlock(addBlock, tempPlacement);
                    addNode(addBlock, tempPlacement);
                }
            }
        }
    }
}

void Maze::placeMaze(mcpp::Coordinate basePoint) {
    std::cout << "Building Maze" << std::endl;
/*
 * Begin Building Maze
 * If alphabet (x), place
 * If symbol (.), do nothing
 * [Test cases for other character done by generation?]
*/
    mcpp::Coordinate placeWall;
    mcpp::Coordinate entrance;
    mcpp::Coordinate editBlocks;
    mcpp::BlockType tempPlacement;
    mcpp::BlockType const ACACIA_WOOD_PLANKS(5,4);
    mcpp::BlockType const BLUE_CARPET(171,11);
    mcpp::BlockType const AIR (0);
    
    mcpp::MinecraftConnection mc;
    bool entranceLocated = false;

   // Begin loops (j = x, i = z) : Go through array putting ACACIA_WOOD_PLANKS for each TRUE.
    // Current x coordinate
    for (int i = basePoint.z; i < basePoint.z + col; ++i) {

        // Current z coordinate
        for (int j = basePoint.x; j < basePoint.x + row; ++j) {

            if (maze[j-basePoint.x][i-basePoint.z]->getWall() == true) {
                // Set placeWall to current detected coordinate
                placeWall.z = i;
                placeWall.y = basePoint.y;
                placeWall.x = j;

                // Place a 3 block high wall
                for (int k = 0; k < 3; ++k) {
                    mc.setBlock(placeWall, ACACIA_WOOD_PLANKS);
                    ++placeWall.y;
                    sleep_for(milliseconds(50));
                }
            }
        }
    }

    // Finding the entrance...
    // Checks each side before setting carpet location one block outside of the entrance.
    for (int i = 0; i < row; ++i && !entranceLocated) {
        if (maze[i][0]->getWall() == false) {
            entrance.x = basePoint.x + i;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z - 1;
            entranceLocated = true;
        } else if (maze[i][row - 1]->getWall() == false) {
            entrance.x = basePoint.x + i;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z + col;
            entranceLocated = true;
        }
    }

    for (int j = 0; j < col; ++j && !entranceLocated) {
        if (maze[0][j]->getWall() == false) {
            entrance.x = basePoint.x - 1;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z + j;
            entranceLocated = true;
        } else if (maze[row - 1][j]->getWall() == false) {
            entrance.x = basePoint.x + row;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z + j;
            entranceLocated = true;
        }
    }



    // Preparing Terrain for Blue Carpet
    if (mc.getHeight(entrance.x, entrance.z) > basePoint.y - 1) {
        editBlocks = entrance;
        while (mc.getHeight(entrance.x, entrance.z) > basePoint.y - 1) {
            editBlocks.y = mc.getHeight(entrance.x, entrance.z);
            addNode(editBlocks, mc.getBlock(editBlocks));
            mc.setBlock(editBlocks, AIR);
        }
    }

    if (mc.getHeight(entrance.x, entrance.z) < basePoint.y - 1) {
        editBlocks = entrance;
        editBlocks.y = mc.getHeight(entrance.x, entrance.y);
        while (mc.getHeight(entrance.x, entrance.z) < basePoint.y - 1) {
            tempPlacement = mc.getBlock(editBlocks);
            editBlocks.y = mc.getHeight(entrance.x, entrance.z) + 1;
            mc.setBlock(editBlocks, tempPlacement);

            // Add the new block to linked list
            addNode(editBlocks, tempPlacement);
        }
    }
    sleep_for(milliseconds(50));
    mc.setBlock(entrance, BLUE_CARPET);
    addNode(entrance, BLUE_CARPET);
}

void Maze::restoreTerrain(mcpp::Coordinate basePoint) {

    mcpp::MinecraftConnection mc;
    mcpp::Coordinate removeBlock;
    mcpp::BlockType const AIR(0);
    mcpp::BlockType const BLUE_CARPET(171,11);
    blockNode* blockHistory;
    
// REMOVE MAZE (Look through Jonas array, remove if wall)
    for (int i = basePoint.z; i < basePoint.z + col; ++i) {

        // Current z coordinate
        for (int j = basePoint.x; j < basePoint.x + row; ++j) {

            if (maze[j-basePoint.x][i-basePoint.z]->getWall() == true) {
                removeBlock.z = i;
                removeBlock.x = j;
                removeBlock.y = mc.getHeight(j, i);

                for (int k = 0; k < 3; ++k) {
                    mc.setBlock(removeBlock, AIR);
                    removeBlock.y = removeBlock.y - 1;
                    sleep_for(milliseconds(50));
                }
            }
        }
    }

// RESTORE TERRAIN (Access coordinates and block id, then add/remove depending on y)
    blockHistory = getNext();
    while (blockHistory != nullptr) {
        
        // ACCESS LINKED LIST
        // If block is above, place | Otherwise, remove
        if (blockHistory->blockID == BLUE_CARPET) {
            mc.setBlock(blockHistory->blockLocation, AIR);
        } else if (blockHistory->blockLocation.y >= basePoint.y) {
            mc.setBlock(blockHistory->blockLocation, blockHistory->blockID);
        } else if (blockHistory->blockLocation.y < basePoint.y) {
            mc.setBlock(blockHistory->blockLocation, AIR);
        }
        blockHistory = getNext();
        sleep_for(milliseconds(50));
    }
}

void Maze::addNode(mcpp::Coordinate blockLocation, mcpp::BlockType blockID) {
    auto newNode = std::make_unique<blockNode>(blockLocation, blockID);

    // If there are nodes in the list, link the new node to the current newestNode
    if (newestNode) {
        newNode->next = std::move(newestNode);
    }

    // Move the new node to newestNode
    newestNode = std::move(newNode);

    // Reset currentNode to the new newestNode
    currentNode = newestNode.get();
}

Maze::blockNode* Maze::getNext() {
    blockNode* placeNode = nullptr;
    // If there is a node, get next, if not, return nullptr
    if (currentNode) {
        placeNode = currentNode; 
        currentNode = currentNode->next.get(); 
    }
    return placeNode; 
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

/*
 * Input: A pointer to a Coordinate object, to dynamically update basePoint 
 * when the user generates multiple mazes in different locations, in a single 
 * running session.
 * 
 * Brief: In normal mode, this method will teleport the player to a random 
 *        location within the maze, avoiding walls, by counting all the air 
 *        blocks present (just for one y-coord level). The bool getStatus() 
 *        Maze method will return true if there is no wall at the current 
 *        location. It then generates a 'true random' integer from the count 
 *        which will be used as the 'anchor' which will be added to the 
 *        basePoint coordinate to teleport the player once found.
 *
 *        In testing mode, this method will teleport the player as far away as 
 *        possible from the basePoint which is two blocks in, both Z and X 
 *        coords from the lower right corner of the maze.
 */
void Maze::solveManually(Coordinate* basePoint) {
    MinecraftConnection mc;
    int airCounter = 0;
    int randAir = 0;
    bool foundRandAir = false;
    Coordinate airLoc(0, 0, 0);

    // Temporariliy for testing
    // Coordinate basePoint(4848, 71, 4369);
    // Coordinate basePoint(115, 74, 153);
    // Coordinate basePoint(150, 74, 137);
    // const int zLen = 7;
    // const int xLen = 6;
    // char maze[zLen][xLen] = {
    //     {"x.xxx"},
    //     {"x.x.x"},
    //     {"x.x.x"},
    //     {"x.x.x"},
    //     {"x.x.x"},
    //     {"x...x"},
    //     {"xxxxx"}
    // };
    // const int zLen = 9;
    // const int xLen = 10;
    // char maze[zLen][xLen] = {
    //     {"xxxxx.xxx"},
    //     {"x.x...x.x"},
    //     {"x.x.xxx.x"},
    //     {"x.x.x...x"},
    //     {"x.x.x.x.x"},
    //     {"x...x.x.x"},
    //     {"x.xxxxx.x"},
    //     {"xxxxxxxxx"}
    // };

    if (this->testMode) {
        airLoc.z += this->col - 2;
        airLoc.x += this->row - 2;
    }
    else {
        for (int i = 0; i < this->col; i++) {
            for (int j = 0; j < this->row; j++) {
                if (maze[i][j]->getStatus()) {
                    airCounter++;
                }
            }
        }

        srand(time(0));
        randAir = rand() % airCounter + 1;
        airCounter = 0;
        for (int i = 0; (i < this->col) && !foundRandAir; i++) {
            for (int j = 0; (j < this->row) && !foundRandAir; j++) {
                if (maze[i][j]->getStatus()) {
                    airCounter++;
                }

                if (airCounter == randAir) {
                    airLoc.z = i + basePoint->z;
                    airLoc.x = j + basePoint->x;

                    // Break out of both loops when found to avoid redundant checking
                    foundRandAir = true;
                }
            }
        }
    }
    airLoc.y = mc.getHeight(airLoc.x, airLoc.z);
    mc.setPlayerTilePosition(airLoc);
}
