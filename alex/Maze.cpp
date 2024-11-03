#include "Maze.h"
#include <thread>
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using mcpp::MinecraftConnection;
using mcpp::Coordinate;
using mcpp::HeightMap;

//TODO Change rows and cols to match canvas after Jonas has changed his

// Construct maze with x rows and y columns
Maze::Maze(int x, int y, bool testMode, bool enhancementMode, Coordinate 
                basePoint) {
    row = x;
    col = y;
    this->testMode = testMode;
    this->enhancementMode = enhancementMode;
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

    // If user selects enhancement mode option from the menu
    if (this->enhancementMode) {
        this->scanTerrain(basePoint);
    }
}

/*
 * Input: A Coodinate basePoint which will be the starting point for checking 
 *        for terrain/obstacles
 * 
 * Brief: This method will scan the area based on the dimensions of the maze
 *        and set each MazeNode's isTerrain data member to true if it can find 
 *        terrain/obstacles that are 2 or more blocks higher than the basePoint
 */

void Maze::scanTerrain(Coordinate basePoint) {
    MinecraftConnection mc;
    /*
     * Using heightmap to store all heights is faster than looking for specific
     * heights with getHeight()
     */
    HeightMap heightMap = mc.getHeights(basePoint, Coordinate(basePoint.x + 
        this->col, basePoint.y, basePoint.z + this->row));
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            /*
             * Check to see if the blocks are 2 or more blocks above the 
             * basePoint or 2 or more blocks below the basePoint
             */
            if ((heightMap.get_worldspace(Coordinate(basePoint.x + j, 
                    // Plus or minus 1 block
                    basePoint.y, basePoint.z + i)) - 1 != 
                    heightMap.get_worldspace(basePoint) 
                    && 
                    heightMap.get_worldspace(Coordinate(basePoint.x + j, 
                    basePoint.y, basePoint.z + i)) + 1 != 
                    heightMap.get_worldspace(basePoint)) 

                    && 
                    // Same height
                    heightMap.get_worldspace(basePoint) != 
                    heightMap.get_worldspace(Coordinate(basePoint.x + j, 
                    basePoint.y, basePoint.z + i))) {

                // Cannot possibly be explored anymore
                this->maze[i][j]->setTerrain(true);
            }
        }
    }
}

void Maze::createGrid() {
    for (int i = 0; i < row; ++i) {
        if (i % 2 != 0) {
            for (int j = 0; j < col; ++j) {
                if (j % 2 != 0) {
                    /*
                     * For enchancement 1 check the terrain first before 
                     * creating the grid with air blocks
                     */
                    if (!maze[i][j]->getTerrain()) {
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
                        // check first col
                        if (j == 1) {
                            maze[i][j]->markLeft();
                        }
                        // check last col
                        if (j == col - 2) {
                            maze[i][j]->markRight();
                        }
                    }
                }
            }
        }
    }
}

MazeNode* Maze::getRandomStart() {
    int x = 0;
    int y = 0;
    MazeNode* start;
    bool startFound = false;
    srand(time(0));

    // assume that if the obstable/ unflattened terrain's is fully in the maze then the maximum size is row - 2 col - 2 bbut on one side can be either row - 1 or col - 1
    // assume that there cannot be obstacles/ unflattened terrain covering the whole maze border (otherwise no entry/exit can be found infi loop)
    while (!startFound) {
        x = (rand() % ((row - 1) / 2)) * 2 + 1;

        if (x == 1 || x == row - 2) {
            y = (rand() % ((col - 1) / 2)) * 2 + 1;
            // CASE: TOP ROW
            if (x == 1) {
                if (!maze[0][y]->getTerrain()) {
                    maze[0][y]->setWall(false);
                    maze[0][y]->setExplored(true);
                    startFound = true;
                }
                
            } else {
            // CASE: BOTTOM ROW
                if (!maze[row - 1][y]->getTerrain()) {
                    maze[row - 1][y]->setWall(false);
                    maze[row - 1][y]->setExplored(true);
                    startFound = true;
                }
                
            }
        } else {
            // random between 0 or 1 (start or end)
            y = rand() % 2;
            // CASE: LEFT
            if (y == 0) {
                if (!maze[x][0]->getTerrain()) {
                    maze[x][0]->setWall(false);
                    maze[x][0]->setExplored(true);
                    startFound = true;
                }
                y = 1;
                
            } else if (y == 1) {
            // CASE: RIGHT
                y = col - 2;
                if (!maze[x][col - 1]->getTerrain()) {
                    maze[x][col - 1]->setWall(false);
                    maze[x][col - 1]->setExplored(true);
                    startFound = true;
                }
            }
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
    if (!next->getStatus() && !next->getTerrain()) {
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
    // createGrid();

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

    // Cleaup if requred to fill in any maze wall blocks
    if (this->enhancementMode) {
        this->checkUnexploredArea();
    }
}

/*
 * Brief: This method is responsible for filling in any blocks within the maze 
 *        that have been left unexplored due to obstacles or terrain 
 */
void Maze::checkUnexploredArea() {
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            if (!this->maze[i][j]->getStatus() && !this->maze[i][j]->getWall() 
                    && !this->maze[i][j]->getTerrain()) {

                this->maze[i][j]->setWall(true);
            }
        }
    }
}

// PLACE MAZE ADJUSTMENT - AS Z INCREASES, BUILD DOWNWARDS, AS X INCREASES, BUILD TO THE RIGHT
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

   // Begin loops (i = x, j = z) : Go through array putting ACACIA_WOOD_PLANKS for each TRUE.
    // Current x coordinate
    for (int i = basePoint.x; i < basePoint.x + col; ++i) {

        // Current z coordinate
        for (int j = basePoint.z; j < basePoint.z + row; ++j) {

            if (maze[j-basePoint.z][i-basePoint.x]->getWall()) {
                // Set placeWall to current detected coordinate
                placeWall.x = i;
                placeWall.y = mc.getHeight(i, j) + 1;
                placeWall.z = j;

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
    for (int i = 0; i < col; ++i && !entranceLocated) {
        if (!maze[0][i]->getWall()) {
            entrance.x = basePoint.x + i;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z - 1;
            entranceLocated = true;
        } else if (!maze[col - 1][i]->getWall()) {
            entrance.x = basePoint.x + i;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z + row;
            entranceLocated = true;
        }
    }

    for (int j = 0; j < row; ++j && !entranceLocated) {
        if (!maze[j][0]->getWall()) {
            entrance.x = basePoint.x - 1;
            entrance.y = basePoint.y;
            entrance.z = basePoint.z + j;
            entranceLocated = true;
        } else if (!maze[j][col - 1]->getWall()) {
            entrance.x = basePoint.x + col;
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
    for (int i = basePoint.x; i < basePoint.x + col; ++i) {

        // Current z coordinate
        for (int j = basePoint.z; j < basePoint.z + row; ++j) {

            if (maze[j-basePoint.z][i-basePoint.x]->getWall() == true) {
                removeBlock.x = i;
                removeBlock.z = j;
                removeBlock.y = mc.getHeight(i, j);

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
}

// Generate maze using user input
void Maze::buildMaze() {
    std::cout << "USER GENERATED MAZE:" << std::endl;
    std::cout << "'x' for wall, '.' for empty space." << std::endl;
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
    std::cout << "Maze generated." << std::endl;
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

    if (this->testMode) {
        airLoc.z += this->row - 2;
        airLoc.x += this->col - 2;
    }
    else {
        for (int i = 0; i < this->row; i++) {
            for (int j = 0; j < this->col; j++) {
                if (this->maze[i][j]->getStatus()) {
                    airCounter++;
                }
            }
        }

        srand(time(0));
        randAir = rand() % airCounter + 1;
        airCounter = 0;
        for (int i = 0; (i < this->row) && !foundRandAir; i++) {
            for (int j = 0; (j < this->col) && !foundRandAir; j++) {
                if (this->maze[i][j]->getStatus()) {
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
