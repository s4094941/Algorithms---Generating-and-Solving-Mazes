#include <chrono>
#include <thread>
#include "Agent.h"
using mcpp::Blocks;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::cout;
using std::endl;

Agent::Agent(Coordinate startLoc) {
    Coordinate currLoc(0, 0, 0);
    srand(time(0));
    agentOrientation orientation = static_cast<agentOrientation>(rand() % 4);
    int xFactor = 0;
    int zFactor = 0;
    int stepCounter = 0;
    bool foundXMinusDir = false;
    bool foundXPlusDir = false;
    bool foundZMinusDir = false;
    bool foundZPlusDir = false;

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
            foundXMinusDir = false;
            foundXPlusDir = false;
            foundZMinusDir = false;
            foundZPlusDir = false;
            xFactor = 0;
            zFactor = 0;

            mc.setBlock(currLoc, Blocks::LIME_CARPET);
            sleep_for(milliseconds(500));
            mc.setBlock(currLoc, Blocks::AIR);
            stepCounter++;
            cout << "Step[" << stepCounter << "]: (" << currLoc.x << ", " << 
                                currLoc.y << ", " << currLoc.z << ")" << endl;

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

            if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                foundXMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                foundXPlusDir = true;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) == Blocks::AIR) {
                foundZMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
                foundZPlusDir = true;
            }

        } while ((mc.getBlock(Coordinate(currLoc.x + xFactor, currLoc.y, 
                                        currLoc.z + zFactor)) == Blocks::AIR) 
                                        && ((!foundXMinusDir) && 
                                        (!foundXPlusDir) && (!foundZMinusDir) 
                                        && (!foundZPlusDir)));

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
            if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) 
                                                            == Blocks::AIR) {
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
    cout << "Step[" << stepCounter << "]: (" << currLoc.x << ", " << 
                                currLoc.y << ", " << currLoc.z << ")" << endl;
    mc.setBlock(currLoc, Blocks::LIME_CARPET);
    sleep_for(milliseconds(500));
    mc.setBlock(currLoc, Blocks::AIR);
}

Agent::~Agent() {

}
