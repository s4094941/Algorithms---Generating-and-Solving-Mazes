#include <chrono>
#include <thread>
#include "Agent.h"
using mcpp::Blocks;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

Agent::Agent(Coordinate startLoc) {
    Coordinate currLoc;
    agentOrientation orientation = X_PLUS;
    int xFactor = 0;
    int zFactor = 0;

    srand(time(0));
    currLoc = startLoc;
    if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) 
                                                            == Blocks::AIR) {
        orientation = X_PLUS;
    }
    else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) 
                                                            == Blocks::AIR) {
        orientation = X_MINUS;
    }
    else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) 
                                                            == Blocks::AIR) {
        orientation = Z_PLUS;
    }
    else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) 
                                                            == Blocks::AIR) {
        orientation = Z_MINUS;
    }
    else {
        orientation = static_cast<agentOrientation>(rand() % 4);
    }

    while (mc.getBlock(Coordinate(currLoc.x + xFactor, currLoc.y, currLoc.z + 
                                    zFactor)) != Blocks::BLUE_CARPET) {
        do {
            xFactor = 0;
            zFactor = 0;
            mc.setBlock(currLoc, Blocks::LIME_CARPET);
            sleep_for(milliseconds(500));
            mc.setBlock(currLoc, Blocks::AIR);
            if (orientation == Z_PLUS) {
                currLoc.z++;
                zFactor++;
            }
            else if (orientation == Z_MINUS) {
                currLoc.z--;
                zFactor--;
            }
            else if (orientation == X_PLUS) {
                currLoc.x++;
                xFactor++;
            }
            else if (orientation == X_MINUS) {
                currLoc.x--;
                xFactor--;
            }
            std::cout << currLoc.x << " " << currLoc.z << std::endl;
        } while (mc.getBlock(Coordinate(currLoc.x + xFactor, currLoc.y, 
                                        currLoc.z + zFactor)) == Blocks::AIR);

        if (orientation == X_PLUS) {
            if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) 
                                                            == Blocks::AIR) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, 
                                                currLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1
                                                        )) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z
                                                        )) == Blocks::AIR) {
                orientation = X_MINUS;
            }
        }
        else if (orientation == X_MINUS) {
            if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) 
                                                            == Blocks::AIR) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, 
                                                currLoc.z)) == Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1
                                                        )) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, 
                                                currLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
        }
        else if (orientation == Z_PLUS) {
            if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) 
                                                            == Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1
                                                        )) == Blocks::AIR) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, 
                                                currLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1
                                                        )) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
        }
        else if (orientation == Z_MINUS) {
            if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) 
                                                            == Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1
                                                        )) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, 
                                                    currLoc.z)) == Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1
                                                        )) == Blocks::AIR) {
                orientation = Z_PLUS;
            }
        }
        else {
            orientation = static_cast<agentOrientation>(rand() % 4);
        }
    }
    mc.setBlock(currLoc, Blocks::LIME_CARPET);
    sleep_for(milliseconds(500));
    mc.setBlock(currLoc, Blocks::AIR);
}

Agent::~Agent() {

}
