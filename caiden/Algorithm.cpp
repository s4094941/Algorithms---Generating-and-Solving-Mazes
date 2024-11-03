#include "Algorithm.h"


void Algorithm::findShortestPath(mcpp::Coordinate playerPos, mcpp::Coordinate exit) {
    // Add player position to OPEN vector

    // Loop - While current node isn't the player exit
        // current = node in OPEN with the lowest f_cost
        // remove current from OPEN
        // add current to CLOSED

        // For - each neighbour of the current node (x+-1, z+-1)
            // If - Neighbour is a wall or neighbour is in CLOSED
                // Skip to the next neighbour

            // If - New path to neighbour is shorter OR neighbour is not in OPEN
                // set f_cost of neighbour
                // set parent of neighbour to current
                // If - Neighbour is not in OPEN
                    // Add Neighbour to OPEN
    

    // return the shortest path vector
}

void Algorithm::placeShortestPath() {

}