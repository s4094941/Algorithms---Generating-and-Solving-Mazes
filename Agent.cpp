#include <chrono>
#include <thread>
#include "Agent.h"
using mcpp::Blocks;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

Agent::Agent(Coordinate startLoc) {
    Coordinate currLoc;
    Coordinate currRightLoc;
    bool setZPLUS = false;
    bool setZMINUS = false;
    bool setXPLUS = false;
    bool setXMINUS = false;
    int xFactor = 0;
    int zFactor = 0;

    currLoc = startLoc;
    while (mc.getBlock(currLoc) != Blocks::LIGHT_BLUE_CARPET) {
        if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
            setXPLUS = true;
        }
        else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
            setXMINUS = true;
        }
        else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
            setZPLUS = true;
        }
        else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) == Blocks::AIR) {
            setZMINUS = true;
        }
        do {
            xFactor = 0;
            zFactor = 0;
            mc.setBlock(currLoc, Blocks::LIME_CARPET);
            sleep_for(milliseconds(500));
            mc.setBlock(currLoc, Blocks::AIR);
            if (setZPLUS) {
                currLoc.z++;
                zFactor++;
            }
            else if (setZMINUS) {
                currLoc.z--;
                zFactor--;
            }
            else if (setXPLUS) {
                currLoc.x++;
                xFactor++;
            }
            else if (setXMINUS) {
                currLoc.x--;
                xFactor--;
                std::cout << "x" << std::endl;
            }
            std::cout << currLoc.x << " " << currLoc.z << std::endl;
        } while (mc.getBlock(Coordinate(currLoc.x + xFactor, currLoc.y, currLoc.z + zFactor)) == Blocks::AIR);

        if (setXPLUS) {
            std::cout << "XPLUS" << std::endl;
            if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
                setZPLUS = true;
                setXPLUS = false;
                setXMINUS = false;
                setZMINUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setXPLUS = true;
                setXMINUS = false;
                setZPLUS = false;
                setZMINUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) == Blocks::AIR) {
                setXPLUS = false;
                setXMINUS = false;
                setZPLUS = false;
                setZMINUS = true;
            }
            else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setXMINUS = true;
                setZPLUS = false;
                setZMINUS = false;
                setXPLUS = false;
            }
        }
        else if (setXMINUS) {
            std::cout << "XMINUS" << std::endl;
            if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
                setZMINUS = false;
                setXPLUS = false;
                setXMINUS = false;
                setZPLUS = true;
            }
            else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setZMINUS = false;
                setXPLUS = false;
                setXMINUS = true;
                setZPLUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
                setXPLUS = false;
                setXMINUS = false;
                setZPLUS = true;
                setZMINUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setXMINUS = false;
                setZPLUS = false;
                setZMINUS = false;
                setXPLUS = true;
            }
        }
        else if (setZPLUS) {
            std::cout << "ZPLUS" << std::endl;
            if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                std::cout << "askhfosaugh" << std::endl;
                setZMINUS = false;
                setXPLUS = false;
                setXMINUS = true;
                setZPLUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
                setZMINUS = false;
                setXPLUS = false;
                setXMINUS = false;
                setZPLUS = true;
            }
            else if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setXPLUS = true;
                setXMINUS = false;
                setZPLUS = false;
                setZMINUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) == Blocks::AIR) {
                setXMINUS = false;
                setZPLUS = false;
                setZMINUS = true;
                setXPLUS = false;
            }
        }
        else if (setZMINUS) {
            std::cout << "ZMINUS" << std::endl;
            if (mc.getBlock(Coordinate(currLoc.x + 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setZMINUS = false;
                setXPLUS = true;
                setXMINUS = false;
                setZPLUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z - 1)) == Blocks::AIR) {
                setZMINUS = true;
                setXPLUS = false;
                setXMINUS = false;
                setZPLUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x - 1, currLoc.y, currLoc.z)) == Blocks::AIR) {
                setXPLUS = false;
                setXMINUS = true;
                setZPLUS = false;
                setZMINUS = false;
            }
            else if (mc.getBlock(Coordinate(currLoc.x, currLoc.y, currLoc.z + 1)) == Blocks::AIR) {
                setXMINUS = false;
                setZPLUS = true;
                setZMINUS = false;
                setXPLUS = false;
            }
        }
    }
}

Agent::~Agent() {

}
