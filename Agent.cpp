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

// Place acacia block, wait, then destroy
void Agent::halfSecDelay() {
    mc.setBlock(this->startLoc, Blocks::LIME_CARPET);
    sleep_for(milliseconds(500));
    mc.setBlock(this->startLoc, Blocks::AIR);
}

// Terminal step output to the console
void Agent::stepOutput(int stepCounter) {
    cout << "Step[" << stepCounter << "]: (" << this->startLoc.x << ", " << 
        this->startLoc.y << ", " << this->startLoc.z << ")" << endl;
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
    agentOrientation currOrientation = static_cast<agentOrientation>(rand() % 4);
    int xFactor = 0;
    int zFactor = 0;
    int stepCounter = 0;
    bool foundXMinusDir = false;
    bool foundXPlusDir = false;
    bool foundZMinusDir = false;
    bool foundZPlusDir = false;

    // If -testing mode flag is enabled
    if (mode) {
        if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = X_PLUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = Z_PLUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y,
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = X_MINUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = Z_MINUS;
        }
        else {
            /*
            * Choose a random direction if the player is in the middle of a '+' 
            * intersection
            */
            currOrientation = static_cast<agentOrientation>(rand() % 4);
        }
    }
    else {
        if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = X_PLUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z - 1)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = Z_MINUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y,
                this->startLoc.z)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = X_MINUS;
        }
        else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
            currOrientation = Z_PLUS;
        }
        else {
            currOrientation = static_cast<agentOrientation>(rand() % 4);
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

            // Terminal step output
            stepCounter++;
            this->stepOutput(stepCounter);

            // Place carpet with a half a second delay in each run of inner loop
            this->halfSecDelay();

            // Move forward and check the block in front of the current block
            if (currOrientation == Z_PLUS) {
                this->startLoc.z++;
                zFactor++;
            }
            else if (currOrientation == Z_MINUS) {
                this->startLoc.z--;
                zFactor--;
            }
            else if (currOrientation == X_PLUS) {
                this->startLoc.x++;
                xFactor++;
            }
            else if (currOrientation == X_MINUS) {
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
                    this->startLoc.y, this->startLoc.z)) != 
                        Blocks::ACACIA_WOOD_PLANK) {
                foundXMinusDir = true;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
                foundZPlusDir = true;
            }

        } while ((mc.getBlock(Coordinate(this->startLoc.x + xFactor, 
                    this->startLoc.y, this->startLoc.z + zFactor)) != 
                    Blocks::ACACIA_WOOD_PLANK) && (!foundXMinusDir && 
                    !foundXPlusDir && !foundZMinusDir && !foundZPlusDir));

        if (currOrientation == X_PLUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                    this->startLoc.z + 1)) != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_PLUS;
            }
            /*
             * For completion, this check has been added to all the respective 
             * current orientations, however there will always be a wall here
             */
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, 
                    this->startLoc.y, this->startLoc.z)) != 
                    Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) != 
                    Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_MINUS;
            }
        }
        else if (currOrientation == X_MINUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y, 
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) != 
                    Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_PLUS;
            }
        }
        else if (currOrientation == Z_PLUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x - 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x + 1, 
                    this->startLoc.y, this->startLoc.z)) != 
                    Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_MINUS;
            }
        }
        else if (currOrientation == Z_MINUS) {
            if (mc.getBlock(Coordinate(this->startLoc.x + 1, this->startLoc.y, 
                    this->startLoc.z))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_PLUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z - 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x - 1, 
                    this->startLoc.y, this->startLoc.z)) != 
                    Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = X_MINUS;
            }
            else if (mc.getBlock(Coordinate(this->startLoc.x, this->startLoc.y,
                    this->startLoc.z + 1))  != Blocks::ACACIA_WOOD_PLANK) {
                currOrientation = Z_PLUS;
            }
        }
        else {
            currOrientation = static_cast<agentOrientation>(rand() % 4);
        }
    }
    this->stepOutput(stepCounter);
    this->halfSecDelay();
}

bool Agent::isNotWall(const mcpp::Coordinate& neighbourCoord) {
    // Walkable = true by default | Comparison to check for walls
    bool walkable = true;
    mcpp::BlockType const ACACIA_WOOD_PLANKS(5,4);

    // If there is a wall, walkable = false
    if (mc.getBlock(neighbourCoord) == ACACIA_WOOD_PLANKS) {
        walkable = false;
    }

    return walkable;
}

void Agent::BFS() {
    // Player Position / Temp Coordinate for neighbouring Nodes / Temp Coordinate for player position
    mcpp::Coordinate playerPos = mc.getPlayerPosition();
    mcpp::Coordinate neighbourNode;
    mcpp::Coordinate currentNode;
    
    // Queue of nodes
    std::queue<mcpp::Coordinate> mazeNodes;

    // Visited coordinates (must all be unique) | Previous Coordinates
    std::unordered_set<mcpp::Coordinate, HashCoordinate> visited;
    std::unordered_map<mcpp::Coordinate, mcpp::Coordinate , HashCoordinate> previous;

    // Comparison
    mcpp::BlockType const BLUE_CARPET(171,11);

    // Iterate a each node in every direction to detect unexplored nodes
    std::vector<mcpp::Coordinate> directions = {
        MOVE_ZPLUS,    
        MOVE_ZMINUS,   
        MOVE_XPLUS,    
        MOVE_XMINUS    
    };

    // Add first/player coordinate to the queue and visited for loop functionality
    mazeNodes.push(playerPos);
    visited.insert(playerPos);

    // Loop while there are nodes to explore, {{and currentNode doesn't have a blue carpet}} ADD THIS LATER
    while (!mazeNodes.empty() && mc.getBlock(currentNode) != BLUE_CARPET) {
        // Checks start of queue, and removes the start of queue node
        currentNode = mazeNodes.front();
        mazeNodes.pop();
        
        // Loop to check every direction
        for (const mcpp::Coordinate& currentDirection : directions) {
            neighbourNode = currentNode;
            neighbourNode.x = neighbourNode.x + currentDirection.x;
            neighbourNode.z = neighbourNode.z + currentDirection.z;

            // Note: Change to a function to decrease line size as suggested
            // If not wall, insert to queue and visited, and add the previous 'path'
            if (isNotWall(neighbourNode) && visited.find(neighbourNode) == visited.end()) {
                mazeNodes.push(neighbourNode);
                visited.insert(neighbourNode);
                previous[neighbourNode] = currentNode;
            }
        }
    }

    // Once blue carpet is found, trace back and get path
    // While the current coordinates aren't the starting point, add to the vector
    while (currentNode != playerPos) {
        shortestPath.push_back(currentNode);
        currentNode = previous[currentNode]; 
    }

    // Add the starting point
    shortestPath.push_back(playerPos);

    // Reverse the vector so that the player position is first, for function functionality
    std::reverse(shortestPath.begin(), shortestPath.end());
}

void Agent::showShortestPath() {
    mcpp::BlockType const LIME_CARPET(171,5);
    mcpp::BlockType const AIR (0);
    // Iterates through the vector, placing a lime carpet to show the path
    for (unsigned int i = 0; i < shortestPath.size() - 1; ++i) {
        mc.setBlock(shortestPath[i], LIME_CARPET);
        sleep_for(milliseconds(500));
        mc.setBlock(shortestPath[i], AIR);
    }
}
