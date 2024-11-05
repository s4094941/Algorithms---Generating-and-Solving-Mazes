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
    MazeNode* getNodeUp(MazeNode*);
    MazeNode* getNodeDown(MazeNode*);
    MazeNode* getNodeLeft(MazeNode*);
    MazeNode* getNodeRight(MazeNode*);

    bool checkNodeUp(MazeNode*);
    bool checkNodeDown(MazeNode*);
    bool checkNodeLeft(MazeNode*);
    bool checkNodeRight(MazeNode*);

    bool checkWallUp(MazeNode*);
    bool checkWallDown(MazeNode*);
    bool checkWallLeft(MazeNode*);
    bool checkWallRight(MazeNode*);


    void checkEdge(MazeNode*);
    MazeNode* findStartPoint();
    MazeNode* findIsolatedNode(bool);
    MazeNode* correctNodePos(MazeNode*);
    MazeNode* probePath(MazeNode*, int, bool&);
    MazeNode* probeWall(MazeNode*, bool&);

public:
    void resetAll();
    void floodFill(MazeNode*);
    void floodFillWall(MazeNode*);

    void connectIsolatedNodes();
    void breakLoops();



//====================================================================================




ADD RESETALL TO BUILDMAZE

//=============== Maze.cpp ============================================================

#include <stack>
#include <vector>
#include <algorithm>
ADD IN CONSTRUCTOR

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
MazeNode* Maze::getNodeUp(MazeNode* n) {
    return maze[n->getRow() - 1][n->getCol()];
}
MazeNode* Maze::getNodeLeft(MazeNode* n) {
    return maze[n->getRow()][n->getCol() - 1];
}

MazeNode* Maze::getNodeDown(MazeNode* n) {
    return maze[n->getRow() + 1][n->getCol()];
}

MazeNode* Maze::getNodeRight(MazeNode* n) {
    return maze[n->getRow()][n->getCol() + 1];
}

// Check whether the node directly adjacent in a particular direction to the provided argument is an unexplored, non-wall node
bool Maze::checkNodeUp(MazeNode* n) {
    bool nodeAvailable = false;

    if (n->getRow() != 0) {
        if (getNodeUp(n)->getWall()   == false
         && getNodeUp(n)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeDown(MazeNode* n) {
    bool nodeAvailable = false;
    if (n->getRow() != row - 1) {
        if (getNodeDown(n)->getWall() == false
         && getNodeDown(n)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeLeft(MazeNode* n) {
    bool nodeAvailable = false;

    if (n->getCol() != 0) {
        if (getNodeLeft(n)->getWall()   == false
         && getNodeLeft(n)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkNodeRight(MazeNode* n) {
    bool nodeAvailable = false;
    if (n->getCol() != col - 1) {
        if (getNodeRight(n)->getWall() == false
         && getNodeRight(n)->getStatus() == false) {
            nodeAvailable = true;
        }
    }

    return nodeAvailable;
}

// Check whether the node directly adjacent in a particular direction to the provided argument is an unexplored, WALL
bool Maze::checkWallUp(MazeNode* n) {
    bool nodeAvailable = false;

    if (n->getRow() > 0) {
        if (getNodeUp(n)->getWall() == false && getNodeUp(n)->getStatus() == false) {
            nodeAvailable == true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkWallDown(MazeNode* n) {
    bool nodeAvailable = false;

    if (n->getRow() < row - 1) {
        if (getNodeDown(n)->getWall() == false && getNodeDown(n)->getStatus() == false) {
            nodeAvailable == true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkWallLeft(MazeNode* n) {
    bool nodeAvailable = false;

    if (n->getCol() > 0) {
        if (getNodeLeft(n)->getWall() == false && getNodeLeft(n)->getStatus() == false) {
            nodeAvailable == true;
        }
    }

    return nodeAvailable;
}
bool Maze::checkWallRight(MazeNode* n) {
    bool nodeAvailable = false;

    if (n->getCol() < row - 1) {
        if (getNodeRight(n)->getWall() == false && getNodeRight(n)->getStatus() == false) {
            nodeAvailable == true;
        }
    }

    return nodeAvailable;
}

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
        if (checkNodeUp(node)) {
            node = getNodeUp(node);
        } else if (checkNodeDown(node)) {
            node = getNodeDown(node);
        } else if (node->getRow() > 1) {
            node = getNodeUp(node); // Move up if possible
            node->setWall(false);
        } else if (node->getRow() < row - 2) {
            node = getNodeDown(node); // Move down if possible
            node->setWall(false);
        }
    } 
    // Check if the node is in an even column
    else if (node->getCol() % 2 == 0) {
        if (checkNodeLeft(node)) {
            node = getNodeLeft(node);
        } else if (checkNodeRight(node)) {
            node = getNodeRight(node);
        } else if (node->getCol() > 1) {
            node = getNodeLeft(node); // Move left if possible
            node->setWall(false);
        } else if (node->getCol() < col - 2) {
            node = getNodeRight(node); // Move right if possible
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
        if (checkNodeUp(curr) == true) {
            getNodeUp(curr)->setExplored(true);
            stack.push(getNodeUp(curr));
        }
        if (checkNodeDown(curr) == true) {
            getNodeDown(curr)->setExplored(true);
            stack.push(getNodeDown(curr));
        }
        if (checkNodeLeft(curr) == true) {
            getNodeLeft(curr)->setExplored(true);
            stack.push(getNodeLeft(curr));
        }
        if (checkNodeRight(curr) == true) {
            getNodeRight(curr)->setExplored(true);
            stack.push(getNodeRight(curr));
        }
    }
}

void Maze::floodFillWall(MazeNode* startPoint) {
    std::stack<MazeNode*> stack;
    stack.push(startPoint);
    startPoint->markWall();

    while (stack.empty() == false) {
        // Set current node to the top of the stack, then pop it off the top of the stack, since it won't need to be checked again.
        // Check nodes in each direction, adding them to the stack and marking them if they are unexplored, non-wall nodes.
        MazeNode* curr = stack.top();
        stack.pop();

        // Check each direction
        if (checkWallUp(curr) == true) {
            getNodeUp(curr)->markWall();
            stack.push(getNodeUp(curr));
        }
        if (checkWallDown(curr) == true) {
            getNodeDown(curr)->markWall();
            stack.push(getNodeDown(curr));
        }
        if (checkWallLeft(curr) == true) {
            getNodeLeft(curr)->markWall();
            stack.push(getNodeLeft(curr));
        }
        if (checkWallRight(curr) == true) {
            getNodeRight(curr)->markWall();
            stack.push(getNodeRight(curr));
        }
    }
}


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
                int dir = isolatedNode->getRandomDirection();
                isolatedNode = probeDirection(isolatedNode, dir, connected);
            } else {
                isolatedNode = isolatedNode->getPrevNode();
            }
        }

        resetAll();
        floodFill(findStartPoint());
        isolatedNode = findIsolatedNode();
    }

    resetAll();
}

// checkDirection for connecting isolated nodes. 
MazeNode* Maze::probeDirection(MazeNode* curr, int dir, bool& connected) {
    // IDEA: make tiers of checks. Only break wall if it cannot jump to another node first.
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

    // if explored == false, then it is connected to the main path
    if (next->getWall() == false && next->getStatus() == false) {
        wall->setWall(false);
        connected = true;
    // if explored == true, it is part of the isolated path, set curr to next. Mark directions.
    } else if (next->getWall() == false && next->getStatus() == true) {
        checkBothDirections(curr, next, dir);
        next->setPrevNode(curr);
        curr = next;
    // if wall, break, set to current.
    } else if (next->getWall() == true) {
        checkBothDirections(curr, next, dir);
        next->setWall(false);
        next->setExplored(true);
        wall->setWall(false);
        wall->setExplored(true);
        next->setPrevNode(curr);
        curr = next;
    }

    return curr;
}

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