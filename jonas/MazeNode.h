#ifndef MAZENODE_H
#define MAZENODE_H

class MazeNode {
    private:
        int row, col;
        int dirCount;
        bool up, left, down, right;
        bool isWall, explored;
        MazeNode* prevNode;

    public:
        // Constructor
        MazeNode(int, int);

        // Mutators
        void setWall(bool);
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
        MazeNode* getPrevNode();

        // Get a random direction from available directions
        // 0 = UP, 1 = DOWN, 2 = LEFT, 3 = RIGHT
        int getRandomDirection();

        // Print node
        void printNode();

};

#endif // MAZENODE_H