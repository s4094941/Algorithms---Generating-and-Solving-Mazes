#include <chrono>
#include <thread>
#include "Agent.h"
using mcpp::Blocks;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::cout;
using std::endl;

Agent::Agent(Coordinate startLoc) {
    this->startLoc = startLoc;
}

Agent::~Agent() {

}

void Agent::rightHandFollow() {
    srand(time(0));
    agentOrientation orientation = static_cast<agentOrientation>(rand() % 4);
    int xFactor = 0;
    int zFactor = 0;
    int stepCounter = 0;
    bool foundXMinusDir = false;
    bool foundXPlusDir = false;
    bool foundZMinusDir = false;
    bool foundZPlusDir = false;

    if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
            this->startLoc.z)) == Blocks::AIR) {
        orientation = X_PLUS;
    }
    else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
            this->startLoc.z)) == Blocks::AIR) {
        orientation = X_MINUS;
    }
    else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
            this->startLoc.z + 1)) == Blocks::AIR) {
        orientation = Z_PLUS;
    }
    else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
            this->startLoc.z - 1)) == Blocks::AIR) {
        orientation = Z_MINUS;
    }
    else {
        orientation = static_cast<agentOrientation>(rand() % 4);
    }

    while (mc.getBlock(Coordinate(this->startLoc.x + xFactor, this->startLoc.y,
        this->startLoc.z + zFactor)) != Blocks::BLUE_CARPET) {
        do {
            foundXMinusDir = false;
            foundXPlusDir = false;
            foundZMinusDir = false;
            foundZPlusDir = false;
            xFactor = 0;
            zFactor = 0;

            mc.setBlock(this->startLoc, Blocks::LIME_CARPET);
            sleep_for(milliseconds(500));
            mc.setBlock(this->startLoc, Blocks::AIR);
            stepCounter++;
            cout << "Step[" << stepCounter << "]: (" << this->startLoc.x << 
                            ", " << this->startLoc.y << ", " << 
                                            this->startLoc.z << ")" << endl;

            if (orientation == Z_PLUS) {
                this->startLoc.z++;
                zFactor++;
            }
            else if (orientation == Z_MINUS) {
                this->startLoc.z--;
                zFactor--;
            }
            else if (orientation == X_PLUS) {
                this->startLoc.x++;
                xFactor++;
            }
            else if (orientation == X_MINUS) {
                this->startLoc.x--;
                xFactor--;
            }

            if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
                    this->startLoc.z)) == Blocks::AIR) {
                foundXMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, 
                    this->startLoc.y, this->startLoc.z)) == Blocks::AIR) {
                foundXPlusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, 
                    this->startLoc.y, this->startLoc.z - 1)) == Blocks::AIR) {
                foundZMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                                    this->startLoc.z + 1)) == Blocks::AIR) {
                foundZPlusDir = true;
            }

        } while ((mc.getBlock(Coordinate(this->startLoc.x + xFactor, 
            this->startLoc.y, this->startLoc.z + zFactor)) == Blocks::AIR) && 
                (!foundXMinusDir && !foundXPlusDir && !foundZMinusDir && 
                    !foundZPlusDir));

        if (orientation == X_PLUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                    this->startLoc.z + 1)) == Blocks::AIR) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, 
                    this->startLoc.y, this->startLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1)) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) == Blocks::AIR) {
                orientation = X_MINUS;
            }
        }
        else if (orientation == X_MINUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                    this->startLoc.z - 1)) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
                    this->startLoc.z)) == Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1)) == Blocks::AIR) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
        }
        else if (orientation == Z_PLUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
                    this->startLoc.z)) == Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1)) == Blocks::AIR) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1)) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
        }
        else if (orientation == Z_MINUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z)) == Blocks::AIR) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1)) == Blocks::AIR) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) == Blocks::AIR) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1)) == Blocks::AIR) {
                orientation = Z_PLUS;
            }
        }
        else {
            orientation = static_cast<agentOrientation>(rand() % 4);
        }
    }
    mc.setBlock(this->startLoc, Blocks::LIME_CARPET);
    sleep_for(milliseconds(500));
    mc.setBlock(this->startLoc, Blocks::AIR);
    cout << "Step[" << stepCounter << "]: (" << this->startLoc.x << ", " << 
        this->startLoc.y << ", " << this->startLoc.z << ")" << endl;
}
