#ifndef MAZENODE_H
#define MAZENODE_H

class MazeNode {
    private:
        int row, col;
        int dirCount;
        bool up, down, left, right;
        bool isWall, explored;
        bool isTerrain;
        MazeNode* prevNode;

    public:
        // Constructor
        MazeNode(int, int);

        // Mutators
        void setWall(bool);
        void setTerrain(bool);
        void setExplored(bool);
        void setPrevNode(MazeNode*);

        // Mark invalid directions
        void markUp();
        void markDown();
        void markLeft();
        void markRight();

        // Accessors
        int getRow();
        int getCol();
        int getDirCount();
        bool getStatus();
        bool getWall();
        bool getTerrain();
        MazeNode* getPrevNode();

        // Get a random direction from available directions
        // 0 = UP, 1 = DOWN, 2 = LEFT, 3 = RIGHT
        int getRandomDirection();
        int getTestDirection();

        // Print node
        void printNode();
        // void placeNode();
};

#endif // MAZENODE_H
