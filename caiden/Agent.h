#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <mcpp/mcpp.h>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using mcpp::MinecraftConnection;
using mcpp::Coordinate;

#define MOVE_XPLUS Coordinate(1, 0, 0)
#define MOVE_XMINUS Coordinate(-1, 0, 0)
#define MOVE_ZPLUS Coordinate(0, 0, 1)
#define MOVE_ZMINUS Coordinate(0, 0, -1)

enum solveAlgorithm {
    RIGHT_HAND_FOLLOW,
    BREATH_FIRST_SEARCH
};

enum agentOrientation {
    X_PLUS,
    Z_PLUS,
    X_MINUS,
    Z_MINUS
};

struct HashCoordinate {
    size_t operator()(const Coordinate& coord) const {
        return (std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1) ^ (std::hash<int>()(coord.z) << 2));
    }
};

class Agent {
    public:
        Agent(Coordinate startLoc);
        ~Agent();
        void rightHandFollow(bool mode);

        // Enhancement
        void BFS();
        void showShortestPath();

    private:
        MinecraftConnection mc;
        Coordinate startLoc;

        void halfSecDelay();
        void stepOutput(int stepCounter);
        void setStartOrientation(agentOrientation currOrientation);

        // Enhancement
        bool isNotWall(const mcpp::Coordinate& coord);
        std::vector<mcpp::Coordinate> shortestPath;
};

#endif // ASSIGN3_AGENT_H
