#include <cstdlib>
#include <iostream>

int main () {
    std::cout << "randomiser test\n";
    srand((unsigned)time(NULL));

    bool active = true;
    char curr = '1';

    while (active) {
        std::cout << "n: new random direction | q: quit program\n";
        std::cin >> curr;
        if (curr == 'n') {
            int dir = (rand() % 4);
            if (dir == 0) {
                std::cout << "< LEFT\n";
            } else if (dir == 1) {
                std::cout << "> RIGHT\n";
            } else if (dir == 2) {
                std::cout << "^ UP\n";
            } else {
                std::cout << "V DOWN\n";
            }
        } else if (curr == 'q') {
            active = false;
        }
    }
}