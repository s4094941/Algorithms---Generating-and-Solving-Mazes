/*
    ASSUMPTIONS:
        Maze edges are intact, only one exit.
        Maze follows correct structure (air nodes are only available in [odd][odd] positions
        Isolated node is not in an even position (will only check for isolated nodes in [odd][odd] positions)
        If isolated node is connected to an isolated path, maze follows correct convention (each turn is only possible 2 nodes away)
        All corners are correctly filled as walls


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
    MazeNode* getNodeLeft(MazeNode*);
    MazeNode* getNodeDown(MazeNode*);
    MazeNode* getNodeRight(MazeNode*);

    bool checkNodeUp(MazeNode*);
    bool checkNodeLeft(MazeNode*);
    bool checkNodeDown(MazeNode*);
    bool checkNodeRight(MazeNode*);

    void checkEdge(MazeNode*);
    MazeNode* findStartPoint();
    MazeNode* peekDirection(MazeNode*, int);

public:
    void resetAll();
    void floodFill(MazeNode*);

    //void findIsolatedNode(MazeNode*);



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
        if (checkNodeLeft(curr) == true) {
            getNodeLeft(curr)->setExplored(true);
            stack.push(getNodeLeft(curr));
        }
        if (checkNodeDown(curr) == true) {
            getNodeDown(curr)->setExplored(true);
            stack.push(getNodeDown(curr));
        }
        if (checkNodeRight(curr) == true) {
            getNodeRight(curr)->setExplored(true);
            stack.push(getNodeRight(curr));
        }
    }

}

void Maze::findIsolatedNode (MazeNode* startPoint) {
    floodFill(startPoint);
    MazeNode* isolatedNode = nullptr;
    bool connected = false;

    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            if (isolatedNode = nullptr
            && maze[i][j]->getStatus() == false && maze[i][j]->getWall() == false) {
                isolatedNode = maze[i][j];
            }
        }
    }

    if (isolatedNode == nullptr) {
        std::cout << "No isolated nodes found." << std::endl;
    }

    resetAll();
    // check edges? Might need to make an edge case for it if checks for the entrance (i.e. don't check the directino if it is on edge and wall == false)
    floodFill(isolatedNode);

    while (connected == false) {
        int dir = isolatedNode->getRandomDirection();
        MazeNode* next = peekDirection(isolatedNode, dir);
        if (next->getExplored() == true) {  // next node is explored (part of the isolated path)

            isolatedNode = next;

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
}

// //====================================================================================