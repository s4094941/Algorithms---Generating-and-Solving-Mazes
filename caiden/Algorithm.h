#ifndef A_Algorithm.h
#define A_Algorithm.h

#include <mcpp/mcpp.h>
#include <set>

class Algorithm {
    private:
        struct AlgorithmNode {
            int g_cost; // Distance from starting node
            int h_cost; // Distance from end node
            std::multiset<int> f_cost; // g + h cost
            mcpp::Coordinate nodeCoordinates; // Coordinates of Node
            AlgorithmNode* parent; // Path node
        };

        std::vector<AlgorithmNode> open;
        std::vector<AlgorithmNode> closed;
        bool test = true;


    public:
        void findShortestPath(mcpp::Coordinate playerPos, mcpp::Coordinate exit);
        void placeShortestPath();
};



#endif