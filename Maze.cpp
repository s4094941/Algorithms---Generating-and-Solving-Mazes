#include "Maze.h"
using mcpp::MinecraftConnection;
using mcpp::Coordinate;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using mcpp::HeightMap;
using std::cout;
using std::endl;

// Construct maze with x rows and y columns
Maze::Maze(int length, int width, bool testMode, bool enhancementMode, Coordinate basePoint) {
    col = length;
    row = width;
    //this -> playerPos = playerPos;
    this->enhancementMode = enhancementMode;
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
                    if (this->enhancementMode) {
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
                    else {
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
    int x = 1;
    int y = 1;
    MazeNode* start;
    bool startFound = false;

    /*
     * For enhancementMode loop until a start location has been found which 
     * means no terrain/obstacles in the way
     */
    if (this->enhancementMode) {
        while (!startFound) {
            x = (rand() % ((row - 1) / 2)) * 2 + 1;

            if (x == 1 || x == row - 2) {
                y = (rand() % ((col - 1) / 2)) * 2 + 1;
                // CASE: TOP ROW
                if (x == 1) {
                    if (!maze[0][y]->getTerrain() && !maze[1][y]->getTerrain()) {
                        maze[0][y]->setWall(false);
                        maze[0][y]->setExplored(true);
                        startFound = true;
                    }
                    
                } else {
                // CASE: BOTTOM ROW
                    if (!maze[row - 1][y]->getTerrain() && !maze[row - 2][y]->getTerrain()) {
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
                    if (!maze[x][0]->getTerrain() && !maze[x][1]->getTerrain()) {
                        maze[x][0]->setWall(false);
                        maze[x][0]->setExplored(true);
                        startFound = true;
                    }
                    y = 1;
                    
                } else if (y == 1) {
                // CASE: RIGHT
                    y = col - 2;
                    if (!maze[x][col - 1]->getTerrain() && !maze[x][col - 2]->getTerrain()) {
                        maze[x][col - 1]->setWall(false);
                        maze[x][col - 1]->setExplored(true);
                        startFound = true;
                    }
                }
            }
        }
    }
    else {
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
    
    if (this->enhancementMode) {
        // Have to check if terrain exists first before carving
        if (!next->getStatus() && !wall->getTerrain()) {
            next->setPrevNode(curr);
            wall->setExplored(true);
            curr=next;
            curr->setExplored(true);
        }
    }
    else {
        if (!next->getStatus()) {
            next->setPrevNode(curr);
            wall->setExplored(true);
            curr=next;
            curr->setExplored(true);
        }
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

    // Cleaup if requred to fill in any maze wall blocks
    if (this->enhancementMode) {
        this->checkUnexploredArea();
    }
    printMaze();
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
// void Maze::buildMaze() {
//     std::cout << "'x' for wall, '.' for empty space.";
//     std::cout << "Note: Entering q will fill the remaining structure with walls." << std::endl;
//     std::cout << "Construct your [" << row << " x " << col << "] structure:" << std::endl;
//     bool quitState = false;
//     char currChar = ' ';
//     for (int i = 0; i < row; ++i) {
//         for (int j = 0; j < col; ++j) {
//             if (!quitState) {
//                 std::cin >> currChar;
//                 if (currChar == 'x') {
//                     maze[i][j]->setWall(true);
//                 } else if (currChar == '.') {
//                     maze[i][j]->setExplored(true);
//                 } else if (currChar == 'q') {
//                     maze[i][j]->setWall(true);
//                     std::cout << "filling in remaining structure with walls." << std::endl;
//                     quitState = true;
//                 } else {
//                     maze[i][j]->setWall(true);
//                     std::cout << "invalid char at [" << i << "][" << j << "]. Defaulted to x" << std::endl;
//                 }
//             } else {
//                 maze[i][j]->setWall(true);
//             }

//         }
//     }

// }

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

    // i = Current x coordinate
    for (int i = basePoint.x; i < basePoint.x + col; ++i) {
        removeBlock.x = i;
        addBlock.x = i;

        // j = Current z coordinate
        for (int j = basePoint.z; j < basePoint.z + row; ++j) {
            removeBlock.z = j;
            addBlock.z = j;

            // If too high, destroy until reaching player's feet, and add destroyed blocks to linked list
            if (mc.getHeight(i, j) > basePoint.y - 1) {
                
                while (mc.getHeight(i, j) > basePoint.y - 1) {
                    removeBlock.y = mc.getHeight(i, j);
                    addNode(removeBlock, mc.getBlock(removeBlock));
                    mc.setBlock(removeBlock, AIR);
                }
            }

            // If too low, add a block (using the block being built upon) and add the block to linked list
            if (mc.getHeight(i, j) < basePoint.y - 1) {
                addBlock.y = mc.getHeight(i, j);

                while (mc.getHeight(i, j) < basePoint.y - 1) {
                    tempPlacement = mc.getBlock(addBlock);
                    addBlock.y = mc.getHeight(i, j) + 1;
                    mc.setBlock(addBlock, tempPlacement);
                    addNode(addBlock, tempPlacement);
                }
            }
        }
    }
}


// PLACE MAZE ADJUSTMENT - AS Z INCREASES, BUILD DOWNWARDS, AS X INCREASES, BUILD TO THE RIGHT
void Maze::placeMaze(mcpp::Coordinate basePoint) {
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

            if (this->enhancementMode) {
                if (maze[j-basePoint.z][i-basePoint.x]->getWall() || maze[j-basePoint.z][i-basePoint.x]->getTerrain()) {
                    // Set placeWall to current detected coordinate
                    placeWall.x = i;
                    placeWall.y = mc.getHeight(i, j) + 1;
                    placeWall.z = j;

                    // Place a blocks high wall
                    for (int k = placeWall.y; k < (basePoint.y + 3); ++k) {
                        mc.setBlock(placeWall, ACACIA_WOOD_PLANKS);
                        mazeBlocks.push_back(placeWall);
                        ++placeWall.y;
                        
                        sleep_for(milliseconds(50));
                    }
                }
            }
            else {
                if (maze[j-basePoint.z][i-basePoint.x]->getWall()) {
                    // Set placeWall to current detected coordinate
                    placeWall.x = i;
                    placeWall.y = basePoint.y;
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
    }

    // Finding the entrance...
    // Checks each side before setting carpet location one block outside of the entrance.

    for (int i = 0; i < col; ++i && !entranceLocated) {
        if (this->enhancementMode) {
            if (!maze[0][i]->getWall() && !maze[0][i]->getTerrain()) {
                entrance.x = basePoint.x + i;
                entrance.z = basePoint.z - 1;
                entrance.y = mc.getHeight(entrance.x, entrance.z) + 1;
                
                entranceLocated = true;
            } else if (!maze[col - 1][i]->getWall() && !maze[col - 1][i]->getTerrain()) {
                entrance.x = basePoint.x + i;
                entrance.z = basePoint.z + row;
                entrance.y = mc.getHeight(entrance.x, entrance.z) + 1;

                entranceLocated = true;
            }
        }
        else {
            if (!maze[0][i]->getWall()) {
                entrance.x = basePoint.x + i;
                entrance.y = basePoint.y;
                entrance.z = basePoint.z - 1;
                // entrance.y = mc.getHeight(entrance.x, entrance.z) + 1;

                entranceLocated = true;
            } else if (!maze[col - 1][i]->getWall()) {
                entrance.x = basePoint.x + i;
                entrance.y = basePoint.y;
                entrance.z = basePoint.z + row;
                // entrance.y = mc.getHeight(entrance.x, entrance.z) + 1;

                entranceLocated = true;
            }
        }
    }

    for (int j = 0; j < row; ++j && !entranceLocated) {
        if (this->enhancementMode) {
            if (!maze[j][0]->getWall() && !maze[j][0]->getTerrain()) {
                entrance.x = basePoint.x - 1;
                entrance.y = basePoint.y;
                entrance.z = basePoint.z + j;
                entranceLocated = true;
            } else if (!maze[j][col - 1]->getWall() && !maze[j][col - 1]->getTerrain()) {
                entrance.x = basePoint.x + col;
                entrance.y = basePoint.y;
                entrance.z = basePoint.z + j;
                entranceLocated = true;
            }
        }
        else {
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
    }

    // Preparing Terrain for Blue Carpet
    if (mc.getHeight(entrance.x, entrance.z) > basePoint.y - 1) {
        editBlocks = entrance;
        while (mc.getHeight(entrance.x, entrance.z) > basePoint.y - 1) {
            editBlocks.y = mc.getHeight(entrance.x, entrance.z);
            addNode(editBlocks, mc.getBlock(editBlocks));
            mc.setBlock(editBlocks, AIR);
            if (this->enhancementMode) {
                mazeBlocks.push_back(editBlocks);
            }
        }
    }

    if (mc.getHeight(entrance.x, entrance.z) < basePoint.y - 1) {
        editBlocks = entrance;
        editBlocks.y = mc.getHeight(entrance.x, entrance.y);
        while (mc.getHeight(entrance.x, entrance.z) < basePoint.y - 1) {
            tempPlacement = mc.getBlock(editBlocks);
            editBlocks.y = mc.getHeight(entrance.x, entrance.z) + 1;
            mc.setBlock(editBlocks, tempPlacement);
            if (this->enhancementMode) {
                mazeBlocks.push_back(editBlocks);
            }

            // Add the new block to linked list
            addNode(editBlocks, tempPlacement);
        }
    }
    sleep_for(milliseconds(50));
    mc.setBlock(entrance, BLUE_CARPET);
    if (this->enhancementMode) {
        mazeBlocks.push_back(entrance);
    }
    addNode(entrance, BLUE_CARPET);
}

void Maze::restoreTerrain(mcpp::Coordinate basePoint) {

    mcpp::MinecraftConnection mc;
    mcpp::Coordinate removeBlock;
    mcpp::BlockType const AIR(0);
    mcpp::BlockType const BLUE_CARPET(171,11);
    blockNode* blockHistory;
    bool allRemoved = false;
    // if (BLUE_CARPET != NULL) {};    
// REMOVE MAZE (Look through Jonas array, remove if wall)
    for (int i = basePoint.x; i < basePoint.x + col && !allRemoved; ++i) {

        // Current z coordinate
        for (int j = basePoint.z; j < basePoint.z + row && !allRemoved; ++j) {

            if (this->enhancementMode) {
                if (maze[j-basePoint.z][i-basePoint.x]->getWall() || maze[j-basePoint.z][i-basePoint.x]->getTerrain()) {
                    removeBlock.x = i;
                    removeBlock.z = j;
                    removeBlock.y = mc.getHeight(i, j);

                    for (size_t k = 0; k < mazeBlocks.size(); k++) {
                        mc.setBlock(mazeBlocks.at(k), AIR);
                        sleep_for(milliseconds(50));
                    }
                    allRemoved = true;
                }
            }
            else {
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

/*
 * Input: A pointer to a Coordinate object, to dynamically update basePoint 
 *        when the user generates multiple mazes in different locations, in a 
 *        single running session.
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

    // If -testing mode flag is enabled
    if (this->testMode) {
        airLoc.z += this->row - 2 + basePoint->z;
        airLoc.x += this->col - 2 + basePoint->x;
    }
    else {
        for (int i = 0; i < this->row; i++) {
            for (int j = 0; j < this->col; j++) {
                // If each MazeNode object is a '.'/air block
                if (this->maze[i][j]->getStatus()) {
                    airCounter++;
                }
            }
        }

        // Make it run randomly each execution
        srand(time(0));
        randAir = rand() % airCounter + 1;
        // Reset the airCounter to count again, until randAir has been reached
        airCounter = 0;
        for (int i = 0; (i < this->row) && !foundRandAir; i++) {
            for (int j = 0; (j < this->col) && !foundRandAir; j++) {
                if (this->maze[i][j]->getStatus()) {
                    airCounter++;
                }

                // Once the random air block has been found set the coordinates
                if (airCounter == randAir) {
                    airLoc.z = i + basePoint->z;
                    airLoc.x = j + basePoint->x;

                    /*
                     * Break out of both loops when found to avoid redundant 
                     * checking
                     */
                    foundRandAir = true;
                }
            }
        }
    }
    airLoc.y = mc.getHeight(airLoc.x, airLoc.z);
    cout << "Teleported to: (4855, 71, 4378)" << endl;
    mc.setPlayerTilePosition(airLoc);

    // Give enough time for the player to load
    sleep_for(milliseconds(1000));
}


// NEW ===================================================================
void Maze::buildMaze() {
    std::cout << "\'x\' for wall, \'.\' for empty space." << std::endl;                             
    std::cout << "Note: Entering q will fill the remaining structure with walls." << std::endl;     
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
}

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

// Check whether the node directly adjacent in a particular direction to the provided argument is valid
    // If argument is a wall, valid means non-explored, wall node
    // if argument is a path, valid means non-explored, path node

bool Maze::checkNodeUp(MazeNode* n, int count) {
    bool nodeAvailable = false;
    
    if (n->getRow() >= 0 + count) {
        MazeNode* next = getNodeUp(n, count);
        if (!next->getStatus()) {
            if ((!n->getWall() && !next->getWall())
            || (n->getWall() && next->getWall())) {
                nodeAvailable = true;
            }
        }
    }

    return nodeAvailable;
}

bool Maze::checkNodeDown(MazeNode* n, int count) {
    bool nodeAvailable = false;
    if (n->getRow() < row - count) {
        MazeNode* next = getNodeDown(n, count);
        if (!next->getStatus()) {
            if ((!n->getWall() && !next->getWall())
            || (n->getWall() && next->getWall())) {
                nodeAvailable = true;
            }
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeLeft(MazeNode* n, int count) {
    bool nodeAvailable = false;

    if (n->getCol() >= 0 + count) {
        MazeNode* next = getNodeLeft(n, count);
        if (!next->getStatus()) {
            if ((!n->getWall() && !next->getWall())
            || (n->getWall() && next->getWall())) {
                nodeAvailable = true;
            }
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeRight(MazeNode* n, int count) {
    bool nodeAvailable = false;
    if (n->getCol() < col - count) {
        MazeNode* next = getNodeRight(n, count);
        if (!next->getStatus()) {
            if ((!n->getWall() && !next->getWall())
            || (n->getWall() && next->getWall())) {
                nodeAvailable = true;
            }
        }
    }

    return nodeAvailable;
}



// Returns the first instance of a non-wall unexplored node.
    // if TRUE search for isolated wall, if FALSE search for isolated path
MazeNode* Maze::findIsolatedNode(bool isWall) {
    MazeNode* node = nullptr;
    bool found = false;
    if (isWall) {
        resetAll();
        floodFill(maze[0][0]);
    } else {
        resetAll();
        floodFill(findStartPoint());
    }

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (!found) {
                if (!isWall) {  // Search for isolated path
                    if (!maze[i][j]->getWall() && !maze[i][j]->getStatus()) {
                        node = maze[i][j];
                        found = true;
                    }
                } else {        // Seach for isolated wall
                    if (maze[i][j]->getWall() && !maze[i][j]->getStatus()) {
                        node = maze[i][j];
                        found = true;
                    }
                }
            }
        }
    }
    return node;
}

// Ensures that a path node is in an [odd][odd] position, and a wall node is in an [even][even] position
MazeNode* Maze::correctNodePos(MazeNode* node) {

    // Fix path node
    if (!node->getWall()) {
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
    // Fix wall node
    } else {
        // check if wall is in an odd row
        if (node->getRow() % 2 == 1) {
            if (checkNodeUp(node, 1)) {
                node = getNodeUp(node, 1);
            } else if (checkNodeDown(node, 1)) {
                node = getNodeDown(node, 1);
            }
        }
        if (node->getCol() % 2 == 1) {
            if (checkNodeLeft(node, 1)) {
                node = getNodeLeft(node, 1);
            } else if (checkNodeRight(node, 1)) {
                node = getNodeRight(node, 1);
            }
        }
    }
    
    return node;
}

// Apologies for how messy this one is.
// Every time I tried cutting it down, either an infinite loop or a segmentation fault would occur somwhere, and I spent hours trying to fix it to no avail.
MazeNode* Maze::probePath(MazeNode* curr, bool& connected) {
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
    for (size_t i = 0; i < dirList.size(); ++i) {
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
        for (size_t i = 0; i < dirList.size(); ++i) {
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
        for (size_t i = 0; i < dirList.size(); ++i) {
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

MazeNode* Maze::probeWall(MazeNode* curr, bool& connected) {
    std::vector<int> dirList;
    bool found = false;

    // Add available directions to dirList and shuffle
    if (!curr->getUp()) dirList.push_back(0);
    if (!curr->getDown()) dirList.push_back(1);
    if (!curr->getLeft()) dirList.push_back(2);
    if (!curr->getRight()) dirList.push_back(3);

    // Shuffle available directions to prevent loops
    std::shuffle(dirList.begin(), dirList.end(), gen);

    // 1. Check each available direction for an unexplored wall node
    for (size_t i = 0; i < dirList.size(); ++i) {
        if (dirList[i] == 0 && !found) {          // Case: Up
            if (checkNodeUp(curr, 2)) {
                getNodeUp(curr, 1)->setWall(true);
                found = true;
                connected = true;
            }
        } else if (dirList[i] == 1 && !found) {   // Case: Down
            if (checkNodeDown(curr, 2)) {
                getNodeDown(curr, 1)->setWall(true);
                found = true;
                connected = true;
            }
        } else if (dirList[i] == 2 && !found) {   // Case: Left
            if (checkNodeLeft(curr, 2)) {
                    getNodeLeft(curr, 1)->setWall(true);
                    found = true;
                    connected = true;
            }
        } else if (dirList[i] == 3 && !found) {   // Case: Right
            if (checkNodeRight(curr, 2)) {
                    getNodeRight(curr, 1)->setWall(true);
                    found = true;
                    connected = true;
            }
        }
    }

    // 2. Jump to random connected wall
    if (!found) {
        for (size_t i = 0; i < dirList.size(); ++i) {
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

// Flood fills using the argument as a starting point. "Marked" nodes are labeled as explored
void Maze::floodFill(MazeNode* startPoint) {
    std::stack<MazeNode*> stack;
   
   stack.push(startPoint);
   startPoint->mark();

    while (stack.empty() == false) {
        // Set current node to the top of the stack, then pop it off the top of the stack, since it won't need to be checked again.
        // Check nodes in each direction, adding them to the stack and marking them if they are unexplored, non-wall nodes.
        MazeNode* curr = stack.top();
        stack.pop();

        // Check each direciton
        if (checkNodeUp(curr, 1) == true) {
            getNodeUp(curr, 1)->mark();
            stack.push(getNodeUp(curr, 1));
        }
        if (checkNodeDown(curr, 1) == true) {
            getNodeDown(curr, 1)->mark();
            stack.push(getNodeDown(curr, 1));
        }
        if (checkNodeLeft(curr, 1) == true) {
            getNodeLeft(curr, 1)->mark();
            stack.push(getNodeLeft(curr, 1));
        }
        if (checkNodeRight(curr, 1) == true) {
            getNodeRight(curr, 1)->mark();
            stack.push(getNodeRight(curr, 1));
        }
    }
}


// Randomly joins any isolated nodes to the main path
void Maze::connectIsolatedNodes () {
    resetAll();
    floodFill(findStartPoint());
    
    // Initial check to find any isolated nodes
    MazeNode* isolatedNode = findIsolatedNode(false);

    // Loop until no nodes exist that are isolated from the main path
    while (isolatedNode) {
        isolatedNode = correctNodePos(isolatedNode);
        resetAll();
        floodFill(isolatedNode);

        bool connected = false;
        while (!connected) {
            if (isolatedNode->getDirCount() > 0) {
                isolatedNode = probePath(isolatedNode, connected);
            } else {
                isolatedNode = isolatedNode->getPrevNode();
            }
        }

        isolatedNode = findIsolatedNode(false);
    }
}

// Builds walls to break any loops
void Maze::breakLoops() {
    MazeNode* isolatedNode = findIsolatedNode(true);

    // Initial check to find any isolated walls (loops)
    if (isolatedNode) {
        floodFill(isolatedNode);
    }

    // Loop until no walls exist that are isolated from the basepoint [0][0]
    while (isolatedNode) {
        isolatedNode = correctNodePos(isolatedNode);

        resetAll();
        floodFill(isolatedNode);

        bool connected = false;
        while (!connected) {
            isolatedNode = probeWall(isolatedNode, connected);
        }

        isolatedNode = findIsolatedNode(true);
        
    }
}

void Maze::validateMaze() {
    bool done = false;

    while (!done) {
        MazeNode* start = findStartPoint();
        floodFill(start);
        floodFill(maze[0][0]);
        done = true;

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (!maze[i][j]->getStatus()) {
                    done = false;
                }
            }
        }

        if (!done) {
            connectIsolatedNodes();
            breakLoops();
        }
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