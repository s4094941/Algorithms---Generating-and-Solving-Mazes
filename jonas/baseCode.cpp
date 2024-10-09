#include <iostream>

int main (void) {

    int row = 10;
    int col = 10;

    // Create dynamic array with size [row] * [col]
    char** structure = new char*[row];

    for (int i = 0; i < row; ++i) {
        structure[i] = new char[col];
    }

    // Fill structure (SQUARE)
    for (int i = 0; i < row; ++i) {
        if (i == 0 || i == row - 1) {
            for (int j = 0; j < col; ++j) {
                structure[i][j] = 'x';
            }
        } else {
            for (int j = 0; j < col; ++j) {
                if (j == 0 || j == col - 1) {
                    structure[i][j] = 'x';
                } else {
                    structure[i][j] = '.';
                }
            }
        }
    }

// ============= DEBUG =====================================
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
// =========================================================


    // Delete structure when done
    for (int i = 0; i < row; ++i) {
        delete[] structure[i];
    }

    delete[] structure;

    return EXIT_SUCCESS;
}