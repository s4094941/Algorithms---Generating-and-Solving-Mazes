#include "MazeNode.h"
#include <cstdlib>
#include <iostream>

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
        void setWall (bool status) {
            isWall = status;
        }
        void setExplored(bool status) {
            explored = status; isWall = false;
        }
        void setPrevNode (MazeNode* prev) {
            prevNode = prev;
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
    
    // Basic accessors
        int getRow() {
            return row;
        }
        int getCol() {
            return col;
        }
        int getDirCount() {
            return dirCount;
        }
        bool getStatus() {
            return explored;
        }
        MazeNode* getPrevNode() {
            if (prevNode != nullptr) { return prevNode; }
            return NULL;
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

        void printNode() {
            if (isWall) {
                std::cout << 'x';
            } else if (explored) {
                std::cout << '.';
            } else {
                std::cout << dirCount;
            }
        }

};