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
    // Default destructor
}

/* Input: A bool mode which tells the method to run as testing or normal mode
 *
 * Brief: In normal mode, this method starts by initialising the lime carpet 
 *        orientation such that it checks its surrounding blocks anti-clockwise
 *        until it reaches the first air block. Then it enters a double loop 
 *        where it first it checkes if the block in front of the current block 
 *        is a blue carpet, then it continues along until the block in front of
 *        the current block is not an an air block i.e. an acacia block. Reason
 *        for checking blocks in front of the current block is to not 
 *        accidently overwrite them. If however an air block is found 
 *        immediately to the right of current block it breaks out of the inner 
 *        loop to perform the orientation change.
 *        Lastly it finishes the carpet placement, by placing down a carpet 
 *        just before the exit.
 *
 *        In testing mode, this method will instead initialise the carpet 
 *        orientation such taht it checks its surrounding blocks clockwise 
 *        until it reaches the first air block.
 */
void Agent::rightHandFollow(bool mode) {
    srand(time(0));
    agentOrientation orientation = static_cast<agentOrientation>(rand() % 4);
    int xFactor = 0;
    int zFactor = 0;
    int stepCounter = 0;
    bool foundXMinusDir = false;
    bool foundXPlusDir = false;
    bool foundZMinusDir = false;
    bool foundZPlusDir = false;

    // If testing mode
    if (mode) {
        if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = X_PLUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = Z_PLUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y,
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = X_MINUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = Z_MINUS;
        }
        else {
            /*
            * Choose a random direction if the player is in the middle of a '+' 
            * intersection
            */
            orientation = static_cast<agentOrientation>(rand() % 4);
        }
    }
    else {
        if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = X_PLUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z - 1)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = Z_MINUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y,
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = X_MINUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
            orientation = Z_PLUS;
        }
        else {
            orientation = static_cast<agentOrientation>(rand() % 4);
        }
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

            // Place carpet with a half a second delay in each run of inner loop
            mc.setBlock(this->startLoc, Blocks::LIME_CARPET);
            sleep_for(milliseconds(500));
            mc.setBlock(this->startLoc, Blocks::AIR);

            // Terminal step output
            stepCounter++;
            cout << "Step[" << stepCounter << "]: (" << this->startLoc.x << 
                ", " << this->startLoc.y << ", " << this->startLoc.z << ")" << 
                    endl;

            // Move forward and check the block in front of the current block
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

            /*
             * If any immediate right location is an air block turn right and 
             * break
             */
            if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
                foundXPlusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, 
                    this->startLoc.y, this->startLoc.z - 1)) != 
                        Blocks::ACACIA_WOOD_PLANK) {
                foundZMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
                foundXMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
                foundZPlusDir = true;
            }

        } while ((mc.getBlock(Coordinate(this->startLoc.x + xFactor, 
            this->startLoc.y, this->startLoc.z + zFactor)) != Blocks::ACACIA_WOOD_PLANK) && 
                (!foundXMinusDir && !foundXPlusDir && !foundZMinusDir && 
                    !foundZPlusDir));

        if (orientation == X_PLUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                    this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_PLUS;
            }
            /*
             * For completion, this check has been added to all the respective 
             * current orientations, however there will always be a wall here
             */
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, 
                    this->startLoc.y, this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1)) != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_MINUS;
            }
        }
        else if (orientation == X_MINUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_PLUS;
            }
        }
        else if (orientation == Z_PLUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_MINUS;
            }
        }
        else if (orientation == Z_MINUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                orientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1))  != Blocks::ACACIA_WOOD_PLANK) {
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
