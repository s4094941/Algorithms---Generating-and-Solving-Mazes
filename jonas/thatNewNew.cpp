/*
    ASSUMPTIONS:
        Maze edges are intact, only one exit.
        Maze follows correct structure (air nodes are only available in [odd][odd] positions
        Isolated node is not in an even position (will only check for isolated nodes in [odd][odd] positions)
        If isolated node is connected to an isolated path, maze follows correct convention (each turn is only possible 2 nodes away)
        All corners are correctly filled as walls
        There must be at least one non-isolated node, not including the exit point
        odd x even or even x odd node can exist, but even x even node cannot.


    FIXES:
        If multiple entry points detected, only first one will count. Any additionals will be turned to wall.
*/

// =============== MazeNode.h ========================================================
private:

public:
    void resetNode();
//====================================================================================



//=============== MazeNode.cpp =======================================================
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

    void checkEdge(MazeNode*);
    MazeNode* findStartPoint();
    MazeNode* probeDirection(MazeNode*, int, bool&);
    void checkBothDirections(MazeNode*, MazeNode*, int );

public:
    void resetAll();
    void floodFill(MazeNode*);

    void connectIsolatedNodes();



//====================================================================================






//=============== Maze.cpp ============================================================

#include <stack>

// Resets all variables to their base values, except row, column, and wall values
// Calls resetNode for each node in the maze
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

// Flood fills using the argument as a starting point. "Marked" nodes are labeled as explored
void Maze::floodFill(MazeNode* startPoint) {
    std::stack<MazeNode*> stack;
    /*
    Stack reminders
        Push(MazeNode*) - add an element to the end of the stack.
            eg: [a, b, c]        push(d) -> [a, b, c, d]
        pop() - remove last element
        isEmpty() returns true for empty stack
        top() returns the current top element without removing it
        size() returns number of elements in stack
    */
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



// iterate through maze array
    // first non-filled node is an isolated node.
        // Reset array, flood fill using isolated node
            // using isolated node as a starting point, check to see if it aligns with odd x odd convention
                // if not, check one block in each direction, and set to current if explored and not wall (if even x even / if even x odd / if odd x even)
                // node can be assumed to be in odd x odd position
            // check 2 blocks in random direction.
                // if wall, break both new node and the wall between, set new node as current node. mark direction as checked for previous node and current node.
                // If explored node, set to current node. mark direction as checked for previous node and current node.
                // If unexplored node, break wall between and end loop.
void Maze::connectIsolatedNodes () {
    MazeNode* startPoint = findStartPoint();
    floodFill(startPoint);
    MazeNode* isolatedNode = nullptr;
    //bool complete = false;

    // Start completed loop here!!
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            if (isolatedNode = nullptr
            && maze[i][j]->getStatus() == false && maze[i][j]->getWall() == false) {
                isolatedNode = maze[i][j];
            }
        }
    }
    // Make node odd x odd. Even x odd or odd x even will always only have odd x odd connected. even x even cannot exist.
    // IF EVEN ROW, CHECK UP OR DOWN
    // IF EVEN COLUMN, CHECK LEFT OR RIGHT

    // repeat until isolatedNode = nullptr

    resetAll();
    // check edges? Might need to make an edge case for it if checks for the entrance (i.e. don't check the directino if it is on edge and wall == false)
    floodFill(isolatedNode);

    // Need to separate joined and connected. There could be a case where two isolated nodes join.
    bool connected = false;
    while (connected == false) {
        // Check random direction.
            // if explored node, then set as current and loop
            // if wall node, set as currrent and loop.
            // if unexplored non-wall node, set connected to true. Flood fill and check for more isolation.
        if (isolatedNode->getDirCount() > 0) {
            int dir = isolatedNode->getRandomDirection();
        } else {
            isolatedNode = isolatedNode->getPrevNode();
        }
        
    }
    // Once connected, reset all and flood fill again and check for more isolation. End once no isolation exists.

    // IDEA: make tiers of checks. Only break wall if it cannot jump to another node first.
}

MazeNode* Maze::probeDirection(MazeNode* curr, int dir, bool& connected) {
    // if explored == true, it is part of the isolated path, set curr to next. Mark directions.
    // if explored == false, then it is connected to the main path
    // if wall, break, set to current.

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

    if (next->getWall() == false && next->getExplored() == false) {
        wall->setWall(false);
        connected = true;
    } else if (next->getWall() == false && next->getExplored() == true) {
        checkBothDirections(curr, next, dir);
        next->setPrevNode(curr);
        curr = next;
    } else if (next->getWall() == true()) {
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
        curr->checkUp();
        next->checkDown();
    //  CHECK DOWN
    } else if (dir == 1) {
        curr->checkDown();
        next->checkUp();
    // CHECK LEFT
    } else if (dir == 2) {
        curr->checkLeft();
        next->checkRight();
    // CHECK RIGHT
    } else if (dir == 3) {
        curr->checkRight();
        next->checkLeft();
    }
}

// //====================================================================================