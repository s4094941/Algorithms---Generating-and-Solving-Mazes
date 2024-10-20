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
        randomDir = rand() % 4;
        if (randomDir == 0) {
            startLoc = Coordinate(startLoc.x + 1, startLoc.y + 1, startLoc.z);
            mc.setBlock(startLoc, Blocks::LIME_CARPET);
        }
        else if (randomDir == 1) {
            startLoc = Coordinate(startLoc.x, startLoc.y + 1, startLoc.z - 1);
            mc.setBlock(startLoc, Blocks::LIME_CARPET);
        }
        else if (randomDir == 2) {
            startLoc = Coordinate(startLoc.x - 1, startLoc.y + 1, startLoc.z);
            mc.setBlock(startLoc, Blocks::LIME_CARPET);
        }
        else if (randomDir == 3) {
            startLoc = Coordinate(startLoc.x, startLoc.y + 1, startLoc.z + 1);
            mc.setBlock(startLoc, Blocks::LIME_CARPET);
        }
        sleep_for(milliseconds(500));
        mc.setBlock(Coordinate(startLoc.x, startLoc.y + 1, startLoc.z), Blocks::AIR);
    }

    currLoc = startLoc;
    while (mc.getBlock(currLoc) != Blocks::LIGHT_BLUE_CARPET) {
        
        mc.setBlock(Coordinate(currLoc.x, currLoc.y + 1, currLoc.z), Blocks::LIME_CARPET);
        sleep_for(milliseconds(500));
        mc.setBlock(Coordinate(currLoc.x, currLoc.y + 1, currLoc.z), Blocks::AIR);

        // Move forward

    }
}

Agent::~Agent() {

}
