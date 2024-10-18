#include "solveManually.h"
using mcpp::MinecraftConnection;

#define NORMAL_MODE 0
#define TESTING_MODE 1

// Input will be a Maze object later
void solveManually() {
    MinecraftConnection mc;
    bool mode = NORMAL_MODE;
    int airCounter = 0;
    int randAir = 0;
    bool foundRandAir = false;
    Coordinate airLoc(0, 0, 0);

    // Temporariliy for testing
    Coordinate basePoint(4848, 71, 4369);
    const int zLen = 7;
    const int xLen = 6;
    char maze[zLen][xLen] = { {"x.xxx"},
                              {"x.x.x"},
                              {"x.x.x"},
                              {"x.x.x"},
                              {"x.x.x"},
                              {"x...x"},
                              {"xxxxx"} };

    if (mode == TESTING_MODE) {
        // airLoc.x = ;
        // airLoc.z = ;
    }
    else {
        for (int i = 0; i < zLen; i++) {
            for (int j = 0; j < xLen; j++) {
                if (maze[i][j] == '.') {
                    airCounter++;
                }
            }
        }

        srand(time(0));
        randAir = rand() % airCounter + 1;
        airCounter = 0;
        for (int i = 0; (i < zLen) && !foundRandAir; i++) {
            for (int j = 0; (j < xLen) && !foundRandAir; j++) {
                if (maze[i][j] == '.') {
                    airCounter++;
                }

                if (airCounter == randAir) {
                    airLoc.z = i + basePoint.z;
                    airLoc.x = j + basePoint.x;
                    foundRandAir = true;
                }
            }
        }
    }
    airLoc.y = mc.getHeight(airLoc.x, airLoc.z);
    mc.setPlayerTilePosition(airLoc);
}
