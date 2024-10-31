#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <mcpp/mcpp.h>
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

class Agent {
    public:
        Agent(Coordinate startLoc);
        ~Agent();
        void rightHandFollow(bool mode);

    private:
        MinecraftConnection mc;
        Coordinate startLoc;

        void halfSecDelay();
        void stepOutput(int stepCounter);
        void setStartOrientation(agentOrientation currOrientation);
};

#endif // ASSIGN3_AGENT_H
