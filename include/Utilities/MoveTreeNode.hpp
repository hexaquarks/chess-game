#pragma once
#include "Move.hpp"

using namespace std;

class MoveTreeNode {
    static inline int numberOfMoves = 0;

    public:
    int childNumber = 0;
    MoveTreeNode* m_parent = nullptr; // To go to previous move
    MoveTreeNode(Move&);   // public for now for debbuging
    Move& m_move; // Key   // public for now for debbuging
    vector<MoveTreeNode*> m_children;   // To to go to next move
    int getNodeLevel();
};
