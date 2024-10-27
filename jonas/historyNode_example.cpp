#include <mcpp>

class HistoryNode {
    private:
        mcpp::Coordinate loc;
        mcpp::Block block;
        HistoryNode* prevNode = nullptr;

    public:
        HistoryNode* getPrev() {
            return prevNode;
        }

        void setPrev(HistoryNode* prev) {
            prevNode = prev;
        }

        resetBlock

        getLoc
        getBlock

}

MAZE: -----
restoreTerrain() {
    while currNode->getPrev!=nullptr() {
        restore
    }
}