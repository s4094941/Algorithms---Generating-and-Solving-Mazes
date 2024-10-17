#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
//#include <MazeNode.h>

class MazeNode {
    private:
        int row, col;
        int dirCount = 4;
        bool up = false, down = false, left = false, right = false;
        bool isWall = true, explored = false;
        MazeNode* prevNode = nullptr;
    // TODO: Change bools to bitfield 000000

    public:
    // Constructor
        MazeNode (int r, int c) : row(r), col(c) {}

    // Basic mutators
        void setWall (bool status) { isWall = status; }
        void setExplored(bool status) { explored = status; isWall = false; }
        void setPrev (MazeNode* prev) { prevNode = prev; }
    
    // Basic accessors
        int getRow() { return row; }
        int getCol() { return col; }
        int getDirCount() { return dirCount; }
        bool getExp() { return explored; }

        void getPrev() {
            if (prevNode == nullptr) {
                std::cout << "No previous node" << std::endl;
            } else {
                std::cout << "Previous node found" << std::endl;
            }
        }

    // Returns the previous node if available
        MazeNode* getPrevNode() {
            if (prevNode != nullptr) { return prevNode; }
            return NULL;
        }

    // Set directions as checked, and decrement number of valid directions
        void markUp()    {
            up    = true; --dirCount;
        }
        void markDown()  {
            down  = true; --dirCount;
        }
        void markLeft()  {
            left  = true; --dirCount;
        }
        void markRight() {
            right = true; --dirCount;
        }

    // Return a random direction from available directions
    // UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;
        int getRandomDirection() {
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

        void print() {
            if (isWall) {
                std::cout << '#';
            } else if (explored) {
                std::cout << '-';
            } else {
                std::cout << dirCount;
            }
        }
};

int main (void) {
    int row;
    int col;
    srand((unsigned)time(NULL));

    std::cout << "Enter the number of rows and columns (must be odd)";
    std::cin >> row >> col;

// Set rows and columns to 0 if input < 0
    if (row < 0) { row = 0; }
    if (col < 0) { col = 0; }

// Ensure rows and columns are odd
    if (row % 2 == 0) { ++row; }
    if (col % 2 == 0) { ++col; }

// Create dynamic array of MazeNodes with size [row] * [col]
    MazeNode*** maze = new MazeNode**[row];
    for (int i = 0; i < row; ++i) {
        maze[i] = new MazeNode*[col];
        for (int j = 0; j < col; ++j) {
            maze[i][j] = new MazeNode(i, j);
        }
    }

    std::cout << "creating a maze with [" << row << "] rows and [" << col << "] columns" << std::endl;

// Set wallStatus to false for all nodes where x and y are odd
    for (int i = 0; i < row; ++i) {
        if (i % 2 != 0) {
            for (int j = 0; j < col; ++j) {
                if (j % 2 != 0) {
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
                    if (j == 1) {
                        maze[i][j]->markLeft();
                    }
                    if (j == col - 2) {
                        maze[i][j]->markRight
            ();
                    }
                }
            }
        }
    }

// Begin randomisation
    int startCol = (rand() % ((col - 1) / 2)) * 2 + 1;
    
    MazeNode* headNode = new MazeNode(-1, -1);
    MazeNode* currNode = maze[1][startCol];
    currNode->setExplored(true);
    currNode->setPrev(headNode);
    maze[0][startCol]->setWall(false);
    maze[0][startCol]->setExplored(true);
    
    while (currNode != headNode) {
    // bool checkDirection(int dir) --------------------
        if (currNode->getDirCount() == 0) {
            currNode = currNode->getPrevNode();
            continue;
        }
        int dir = currNode->getRandomDirection();
    // check node 2 blocks UP
        if (dir == 0) {
            currNode->markUp();
            MazeNode* nextNode =  maze[currNode->getRow() - 2][currNode->getCol()];
            MazeNode* nextWall = maze[currNode->getRow() - 1][currNode->getCol()];
            if (nextNode->getExp() == false) {
                nextNode->setPrev(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks DOWN            
        } else if (dir == 1) {
            currNode->markDown();
            MazeNode* nextNode =  maze[currNode->getRow() + 2][currNode->getCol()];
            MazeNode* nextWall = maze[currNode->getRow() + 1][currNode->getCol()];
            if (nextNode->getExp() == false) {
                nextNode->setPrev(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks LEFT            
        } else if (dir == 2) {
            currNode->markLeft();
            MazeNode* nextNode =  maze[currNode->getRow()][currNode->getCol() - 2];
            MazeNode* nextWall = maze[currNode->getRow()][currNode->getCol() - 1];
            if (nextNode->getExp() == false) {
                nextNode->setPrev(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
    // check node 2 blocks RIGHT
        } else if (dir == 3) {
            currNode->markRight();
            MazeNode* nextNode =  maze[currNode->getRow()][currNode->getCol() + 2];
            MazeNode* nextWall = maze[currNode->getRow()][currNode->getCol() + 1];
            if (nextNode->getExp() == false) {
                nextNode->setPrev(currNode);
                currNode = nextNode;
                currNode->setExplored(true);
                nextWall->setExplored(true);
                //delete nextNode;
            }
        }
    }

// Print maze

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            maze[i][j]->print();
        }
        std::cout << std::endl;
    }

// Delete maze when done -----
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            delete maze[i][j];
        }
        delete[] maze[i];
    }

    delete[] maze;

    return EXIT_SUCCESS;
}

// Create linked list for current path
// Check 2 spaces in random direction
    // if node, break wall
    // else, pop the top node off, check that direction
        // loop
// end when vector.size() == 0

// Random start/exit points

// Random direction that HASN'T been explored

// valgrind --tool=memcheck --leak-check=yes ./test < env.input
    // env.input is just the saved input