//Maze.h ============================================================================
#include <stack>

MazeNode* getNodeUp(MazeNode*);
MazeNode* getNodeLeft(MazeNode*);
MazeNode* getNodeDown(MazeNode*);
MazeNode* getNodeRight(MazeNode*);

bool checkNodeUp(MazeNode*);
bool checkNodeLeft(MazeNode*);
bool checkNodeDown(MazeNode*);
bool checkNodeRight(MazeNode*);

void floodFill(MazeNode*);





//====================================================================================






// Maze.cpp ==========================================================================

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
   

}

// //====================================================================================