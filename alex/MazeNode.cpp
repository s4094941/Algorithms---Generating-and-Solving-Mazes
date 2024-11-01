#include "MazeNode.h"
#include <cstdlib>
#include <iostream>

// Constructor
MazeNode::MazeNode(int r, int c) : row(r), col(c), dirCount(4), up(false), down(false), left(false), right(false),
                                   isWall(true), explored(false), prevNode(nullptr) {}

// Basic mutators
void MazeNode::setWall (bool status) {
    isWall = status;
    if (status == true) {
        explored = true;
    }
}

void MazeNode::setTerrain(bool status) {
    // Set wall to false as it was initialised to true by default
    this->isWall = false;
    this->isTerrain = status;
    if (status) {
        this->explored = true;
    }
}

void MazeNode::setExplored(bool status) {
    explored = status;
    if (status == true) {
        isWall = false;
    }
}
void MazeNode::setPrevNode (MazeNode* prev) {
    prevNode = prev;
}
// Set directions as checked, and decrement number of valid directions
void MazeNode::markUp()    {
    up    = true; --dirCount;
}
void MazeNode::markDown()  {
    down  = true; --dirCount;
}
void MazeNode::markLeft()  {
    left  = true; --dirCount;
}
void MazeNode::markRight() {
    right = true; --dirCount;
}

// Basic accessors
int MazeNode::getRow() {
    return row;
}
int MazeNode::getCol() {
    return col;
}
int MazeNode::getDirCount() {
    return dirCount;
}
bool MazeNode::getStatus() {
    return explored;
}
bool MazeNode::getWall() {
    return isWall;
}

bool MazeNode::getTerrain() {
    return this->isTerrain;
}

MazeNode* MazeNode::getPrevNode() {
    if (prevNode != nullptr) { return prevNode; }
    return NULL;
}

// Return a random direction from available directions
// UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;
int MazeNode::getRandomDirection() {
    if (dirCount <= 0) { return -1; }

    int count = 0;
    int dirList[4];
    if (!up) {
        dirList[count++] = 0;
    }
    if (!down) {
        dirList[count++] = 1;
    }
    if (!left) {
        dirList[count++] = 2;
    }
    if (!right) {
        dirList[count++] = 3;
    }

    int direction = dirList[rand() % dirCount];

    return direction;
}

int MazeNode::getTestDirection() {
    if (dirCount <= 0) { return -1; }

    if (up == false) {
        return 0;
    } else if (right == false) {
        return 3;
    } else if (down == false) {
        return 1;
    } else {
        return 2;
    }
}

// Print Node
void MazeNode::printNode() {
    if (isWall) {
        std::cout << 'x';
    }
    else if (this->isTerrain) {
        std::cout << '#';
    }
    else if (explored) {
        std::cout << '.';
    }
    else {
        std::cout << dirCount;
    }
}
