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

    }

    currLoc = startLoc;
    do {
        
        // Sleep function
        sleep_for(milliseconds(500));

    } while (mc.getBlock(currLoc) != Blocks::LIGHT_BLUE_CARPET);
}

Agent::~Agent() {

}
