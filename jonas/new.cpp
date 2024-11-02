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


    FIXES:
        If multiple entry points detected, only first one will count. Any additionals will be turned to wall.
*/