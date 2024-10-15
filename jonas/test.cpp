#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
//#include <MazeNode.h>

class MazeNode {
    private:
        int row, col;
        bool up, left, down, right, isWall;
        bool explored = false;
        MazeNode* prevNode = nullptr;
        // TODO: Change bools to bitfield 000000
    public:
        MazeNode (int r, int c) : row(r), col(c) {
            isWall = true;
            up = false;
            left = false;
            down = false;
            right = false;
        }
        void setWall (bool status) {
            isWall = status;
        }

        void print() {
            if (isWall) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }

        int getRow() { return row; }
        int getCol() { return col; }
        bool getExp() { return explored; }
        MazeNode* getPrevNode() { return prevNode; }

        ~MazeNode() {
            delete this;
        }
};

int main (void) {
    int row;
    int col;

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
                    maze[i][j]->setWall(false);
                }
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

    //bool complete = false;
    srand((unsigned)time(NULL));
    int startPoint = rand() % col;
            std::cout << "StartPoint = " << startPoint; // DEBUG

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



// NODE
    // x, y
    // wall/air
    // explored y/n
    // prev node, if no, exit
    // bool for UP, LEFT, DOWN, RIGHT
        // randomly check unexplored direction