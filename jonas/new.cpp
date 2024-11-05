/*
    ASSUMPTIONS:
        Row and column must both be positive odd integers. If a value below 3 is passed, it will be converted to 3.
        Any even values will have 1 added to them such that they are odd.

        A maze is built up of wall nodes, path nodes, and path connection nodes.
            - Only one exit node can exist, which is a path node on the outer edge of the maze.
            - By default, path nodes can only be on [odd][odd] positions (eg. maze[1][1], which is technically the 2nd row and column).
                * As such, any isolated wall nodes encapsulated by a loop must have an [even][even] node within the cluster.
            - In non-user-generated modes, [odd][odd] nodes will always be path nodes, and [even][even] nodes will always be wall nodes.
        
        In user-generated modes:
            - [odd][odd] nodes may be walls, but [even][even] nodes must still be walls.
            - Maze edges must be all walls, with only one exit node. Any additional exit nodes will be converted to walls.
            - Exit points must be connected to at least one path node.
            - All corners must be filled in with walls.
            - The main path stemming from the exit point must follow regular maze conventions.
        
    FIXES:
        If multiple entry points detected, only first one will count. Any additionals will be turned to wall.
        If isolated wall is found in a node connection (odd x even or even x odd), it will correct itself to an odd x odd node
        If multiple walls are between an isolated path node and the main path, the program will randomly tunnel through walls until they connect
*/

// =============== MazeNode.h ========================================================

private:

public:
    void mark();
    void checkWall();
    void resetNode();
    bool getUp();
    bool getDown();
    bool getLeft();
    bool getRight();
//====================================================================================



//=============== MazeNode.cpp =======================================================
bool MazeNode::getUp() {
    return up;
}
bool MazeNode::getDown() {
    return down;
}
bool MazeNode::getLeft() {
    return left;
}
bool MazeNode::getRight() {
    return right;
}
void MazeNode::checkWall() {
    if (isWall) {
        explored = true;
    }
}

void MazeNode::mark() {
    explored = true;
}

void MazeNode::resetNode() {
    up = false;
    down = false;
    left = false;
    right = false;
    explored = false;
    prevNode = nullptr;
    dirCount = 4;
    explored = false;
}
//====================================================================================



//=============== Maze.h =============================================================

private:
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
    void resetAll();
    void floodFill(MazeNode*);
    void connectIsolatedNodes();
    void breakLoops();
    void validateMaze();



//====================================================================================





//=============== Maze.cpp ============================================================

#include <stack>
#include <vector>
#include <algorithm>

//ADD IN CONSTRUCTOR:
    std::random_device rd;
    gen.seed(rd());


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
    resetAll();
    printMaze();
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
    for (int i = 0; i < dirList.size(); ++i) {
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

// //====================================================================================