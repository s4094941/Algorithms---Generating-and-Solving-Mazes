#include "Agent.h"
using mcpp::Blocks;

Agent::Agent(Coordinate startLoc) {
    agentOrientation orientation;
    agentOrientation currOrientation;
    Coordinate currLoc;
    Coordinate currRightLoc;
    Coordinate rightLocForward;

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
        rightLocForward = currRightLoc;
    } while ((mc.getBlock(currRightLoc) != Blocks::AIR) && (mc.getBlock(rightLocForward) != Blocks::AIR));
}

Agent::~Agent() {

}
