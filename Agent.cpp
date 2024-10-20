#include <chrono>
#include <thread>
#include "Agent.h"
using mcpp::Blocks;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

Agent::Agent(Coordinate startLoc) {
    agentOrientation orientation;
    agentOrientation currOrientation;
    Coordinate currLoc;
    Coordinate currRightLoc;
    int randomDir = 0;

    if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
        orientation = X_PLUS;
    }
    else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
        orientation = Z_MINUS;
    }
    else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
        orientation = X_MINUS;
    }
    else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
        orientation = Z_PLUS;
    }
    else {
        srand(time(0));
        randomDir = rand() % 4;
        if (randomDir == 0) {
            startLoc = Coordinate(startLoc.x + 1, startLoc.y, startLoc.z);
            if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                orientation = Z_PLUS;
            }
        }
        else if (randomDir == 1) {
            startLoc = Coordinate(startLoc.x, startLoc.y, startLoc.z - 1);
            if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y + 1, startLoc.z)) != Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                orientation = Z_PLUS;
            }
        }
        else if (randomDir == 2) {
            startLoc = Coordinate(startLoc.x - 1, startLoc.y, startLoc.z);
            if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                orientation = Z_PLUS;
            }
        }
        else if (randomDir == 3) {
            startLoc = Coordinate(startLoc.x, startLoc.y, startLoc.z + 1);
            if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                orientation = Z_PLUS;
            }
        }
    }

    currLoc = startLoc;
    while (mc.getBlock(currLoc) != Blocks::LIGHT_BLUE_CARPET) {
        mc.setBlock(currLoc, Blocks::LIME_CARPET);
        sleep_for(milliseconds(500));
        mc.setBlock(currLoc, Blocks::AIR);

        if (orientation == X_PLUS) {
            currLoc.x++;
        }
        else if (orientation == Z_MINUS) {
            currLoc.z--;
        }
        else if (orientation == X_MINUS) {
            currLoc.x--;
        }
        else if (orientation == Z_PLUS) {
            currLoc.z++;
        }

        // Ctrl + H all the startLoc below to currLoc

        if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) != Blocks::AIR) {
        orientation = X_PLUS;
        }
        else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) != Blocks::AIR) {
            orientation = Z_MINUS;
        }
        else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) != Blocks::AIR) {
            orientation = X_MINUS;
        }
        else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) != Blocks::AIR) {
            orientation = Z_PLUS;
        }
        else {
            srand(time(0));
            randomDir = rand() % 4;
            if (randomDir == 0) {
                currLoc = Coordinate(currLoc.x + 1, currLoc.y, currLoc.z);
                if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) != Blocks::AIR) {
                    orientation = X_PLUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                    orientation = Z_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                    orientation = X_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                    orientation = Z_PLUS;
                }
            }
            else if (randomDir == 1) {
                startLoc = Coordinate(startLoc.x, startLoc.y, startLoc.z - 1);
                if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                    orientation = X_PLUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                    orientation = Z_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y + 1, startLoc.z)) != Blocks::AIR) {
                    orientation = X_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                    orientation = Z_PLUS;
                }
            }
            else if (randomDir == 2) {
                startLoc = Coordinate(startLoc.x - 1, startLoc.y, startLoc.z);
                if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                    orientation = X_PLUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                    orientation = Z_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                    orientation = X_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                    orientation = Z_PLUS;
                }
            }
            else if (randomDir == 3) {
                startLoc = Coordinate(startLoc.x, startLoc.y, startLoc.z + 1);
                if (mc.getBlock(Coordinate(startLoc.x + 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                    orientation = X_PLUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z - 1)) != Blocks::AIR) {
                    orientation = Z_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x - 1, startLoc.y, startLoc.z)) != Blocks::AIR) {
                    orientation = X_MINUS;
                }
                else if (mc.getBlock(Coordinate(startLoc.x, startLoc.y, startLoc.z + 1)) != Blocks::AIR) {
                    orientation = Z_PLUS;
                }
            }
        }
    }
}

Agent::~Agent() {

}
