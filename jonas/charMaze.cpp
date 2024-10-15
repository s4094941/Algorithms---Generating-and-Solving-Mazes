#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

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

// Create dynamic array with size [row] * [col]
    char** structure = new char*[row];

    for (int i = 0; i < row; ++i) {
        structure[i] = new char[col];
    }

    std::cout << "creating a structure with [" << row << "] rows and [" << col << "] columns" << std::endl;

// . = untouched node
// x = wall
// - = path


    for (int i = 0; i < row; ++i) {
        if (i % 2 == 0) {
            for (int j = 0; j < col; ++j) {
                structure[i][j] = '#';
            }
        } else {
            for (int j = 0; j < col; ++j) {
                if (j % 2 == 0) {
                    structure[i][j] = '#';
                } else {
                    structure[i][j] = '.';
                }
            }
        }
    }

// Print structure
    for (int i = 0; i < row; ++i) {
        //std::cout << "?";
        for (int j = 0; j < col; ++j) {
            //std::cout << "!";
            std::cout << structure[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;



// Create vector of pairs
    std::vector<std::pair<int, int>> path;

//
    //bool complete = false;
    srand((unsigned)time(NULL));
    int startPoint = rand() % col;
            std::cout << "StartPoint = " << startPoint; // DEBUG

// Delete structure when done -----
    for (int i = 0; i < row; ++i) {
        delete[] structure[i];
    }

    delete[] structure;

    return EXIT_SUCCESS;
}

// TODO: every even row/column will be air
    // fill w air
    // fill all x % 2 == 1 with wall
    // fill all y % 2 == 1 with wall

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