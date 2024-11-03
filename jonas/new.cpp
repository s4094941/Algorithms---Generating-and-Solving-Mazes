// TO DO LIST:
    // for get start point, mark direction to check as invalid based on position (maybe unnecessary)


// Maze.h ================================================
private:
    void resetAll();
    void checkEdge(MazeNode*);
    MazeNode* findStartPoint();
    MazeNode* checkAdjacent(MazeNode*, int);
    void checkConnections();
    void findIsolatedNode();

    MazeNode* getNodeUp(MazeNode*);
    MazeNode* getNodeLeft(MazeNode*);
    MazeNode* getNodeDown(MazeNode*);
    MazeNode* getNodeRight(MazeNode*);

    bool checkNodeUp(MazeNode*);
    bool checkNodeLeft(MazeNode*);
    bool checkNodeDown(MazeNode*);
    bool checkNodeRight(MazeNode*);

public:
    void correctGrid();
    void checkValidity();

// Maze.cpp
void Maze::resetAll() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            maze[i][j]->resetNode();
            checkEdge(maze[i][j]);
        }
    }
}

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

MazeNode* Maze::findStartPoint() {
    MazeNode* startPoint = nullptr;

    // CHECK TOP ROW
    for (int i = 1; i < row - 1; ++i) {
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
    for (int i = 1; i < row - 1; ++i) {
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
    for (int i = 1; i < col - 1; ++i) {
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
    for (int i = 0; i < col - 1; ++i) {
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

// Ensures that the maze is created with the correct layout
void Maze::correctGrid() {
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            if (i % 2 == 1 && j % 2 == 1) {
                maze[i][j]->setWall(false);
            }
        }
    }
}

void Maze::checkConnections(MazeNode* startNode) { // Could also be checkConnections
    MazeNode* headNode = new MazeNode(-1, -1);
    MazeNode* currNode = startNode;
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
            currNode = checkAdjacent(currNode, dir);
        }
    }
}


MazeNode* Maze::checkAdjacent(MazeNode* curr, int dir) {
    // Row offset and Column offset
    int ros = 0;
    int cos = 0;

    if (dir == 0) {
        ros = -1;
    } else if (dir == 1) {
        ros = 1;
    } else if (dir == 2) {
        cos = -1;
    } else if (dir == 3) {
        cos = 1;
    }

    // Wall row offset and Wall column offset = half of the regular offset
    int wros = ros / 2;
    int wcos = cos / 2;

    MazeNode* next = maze[curr->getRow() + ros][curr->getCol() + cos];
    if (next->getStatus() == false) {
        next->setPrevNode(curr);
        curr=next;
        curr->setExplored(true);
    }
    return curr;
}

void findIsolatedNode() {
    // Find startNode
    // checkConnections

    // TODO: Change to only check odd columns and rows (i/j += 2 ?)

    for (int i = 1; i < row - 1; i += 2) {
        for (int j = 1; j < col - 1; j += 2) {
            if (maze[i][j]->getWall == false && maze[i][j]->getStatus == false) {
                // This node is isolated
                    // Reset all nodes
                    // Flood fill node, using this node as startNode
                    // From there, check node in each direction. If it is unexplored, break wall. This can be a wall, as it would fall into the correct conventions for an air node.


                    // REDO flood fill to not check off directions
                        // check up, left, down, right. If any is air, mark checked, and set air as current.
                            // if no air available, backtrack.
                     
            }
        }
    }
}

// Returns a pointer to the node directly adjacent in a particular direction
MazeNode* Maze::getNodeUp(MazeNode* n) {
    return maze[n->getRow() - 1][n->getCol()]
}
MazeNode* Maze::getNodeLeft(MazeNode* n) {
    return maze[n->getRow()][n->getCol() - 1]
}

MazeNode* Maze::getNodeDown(MazeNode* n) {
    return maze[n->getRow() + 1][n->getCol()]
}

MazeNode* Maze::getNodeRight(MazeNode* n) {
    return maze[n->getRow()][n->getCol() + 1]
}

// Check whether the node directly adjacent in a particular direction is an unexplored, non-wall node
bool Maze::checkNodeUp(MazeNode* n) {
    bool nodeAvailable = false;
    if (n->getRow() != 0
    && getNodeUp(n)->getWall() == false
    && getNodeUp(n)->getStatus() == false) {
        nodeAvailable = true;
    }
    return nodeAvailable
}
bool Maze::checkNodeLeft(MazeNode* n) {
    bool nodeAvailable = false;
    if (getNodeLeft(n)->getWall == false
     && getNodeLeft(n)->getStatus == false) {
        nodeAvailable = true;
    }
    return nodeAvailable
}
bool Maze::checkNodeDown(MazeNode* n) {
    bool nodeAvailable = false;
    if (getNodeDown(n)->getWall == false
     && getNodeDown(n)->getStatus == false) {
        nodeAvailable = true;
    }
    return nodeAvailable
}
bool Maze::checkNodeRight(MazeNode* n) {
    bool nodeAvailable = false;
    if (getNodeRight(n)->getWall == false
     && getNodeRight(n)->getStatus == false) {
        nodeAvailable = true;
    }
    return nodeAvailable
}


void Maze::floodFill(MazeNode* startPoint) {
    MazeNode* curr = startPoint;
    curr->setExplored(true);
    bool filled = false;

    // TEST CASE FOR OPENING, BUT NO CONNECTED NODES
    if (curr->getRow() == 0) {
        // if current is on row 0, check down, if not a wall, make curr, else filled = true
        if (getNodeDown(curr)->getWall == false) {
            getNodeDown(curr)->setPrevNode(curr);
            curr = getNodeDown(curr);
            curr->setExplored(true);
        } else {
            filled = true;
        }
    } else if (curr->getCol() == 0) {
        // else if current is on column 0, check right, if not a wall, make curr, else filled = true
        if (getNodeRight(curr)->getWall == false) {
            getNodeRight(curr)->setPrevNode(curr);
            curr = getNodeRight(curr);
            curr->setExplored(true);
        } else {
            filled = true;
        }
    } else if (curr->getRow() == row - 1) {
        // else if current is on row row - 1, check up, if not a wall, make curr, else filled = true
        if (getNodeUp(curr)->getWall == false) {
            getNodeUp(curr)->setPrevNode(curr);
            curr = getNodeUp(curr);
            curr->setExplored(true);
        } else {
            filled = true;
        }
    } else if (curr->getCol() == col - 1) {
        // else if current is on column column - 1, check left, if not a wall, make curr, else filled = true
        if (getNodeLeft(curr)->getWall == false) {
            getNodeLeft(curr)->setPrevNode(curr);
            curr = getNodeLeft(curr);
            curr->setExplored(true);
        } else {
            filled = true;
        }
    }

    while (curr != nullptr && filled != false) {
        if (checkNodeUp(curr) == true) {
            getNodeUp(curr)->setPrevNode(curr);
            curr = getNodeUp(curr);
            curr->setExplored(true);
        } else if (checkNodeLeft(curr) == true) {
            getNodeLeft(curr)->setPrevNode(curr);
            curr = getNodeLeft(curr);
            curr->setExplored(true);
        } else if (checkNodeDown(curr) == true) {
            getNodeDown(curr)->setPrevNode(curr);
            curr = getNodeDown(curr);
            curr->setExplored(true);
        } else if (checkNodeRight(curr) == true) {
            getNodeRight(curr)->setPrevNode(curr);
            curr = getNodeRight(curr);
            curr->setExplored(true);
        } else {
            curr = curr->getPrevNode();
        }
    }
}



void Maze::checkValidity() {
    MazeNode* startNode = nullptr;
    MazeNode* curr = nullptr;
    
    resetAll();
    startNode = findStartPoint();
    curr = startNode;

    while (curr != nullptr) {

    }

    // While !complete
        // checkConnections

}

/*
checkConnections
    begin at startPoint
    check node up 1, if so, next
        mark up on curr
            mark down on next
            mark explored on next
            curr = next

    check node left 1, if so, next
    check node right 1, if so, next = curr
    check node down 1, if so, next = curr
        if no available node
*/


// mazeNode.h ==============================================
private:

public:
    void resetNode();

// MazeNode.cpp
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