#include <iostream>

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
        if (i == 0 || i == row - 1) {
            for (int j = 0; j < col; ++j) {
                structure[i][j] = 'x';
            }
        } else {
            for (int j = 0; j < col; ++j) {
                if (j % 2 == 0) {
                    structure[i][j] = 'x';
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

// Delete structure when done -----
    for (int i = 0; i < row; ++i) {
        delete[] structure[i];
    }

    delete[] structure;

    return EXIT_SUCCESS;
}