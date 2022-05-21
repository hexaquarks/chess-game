#pragma once
#include <memory>
#include "Move.hpp"

using namespace std;

class MoveTreeNode
{
    static inline int numberOfMoves = 0;

    public:
    int childNumber = 0;
    shared_ptr<MoveTreeNode> m_parent; // To go to previous move
    MoveTreeNode(shared_ptr<Move>&); // public for now for debbuging
    MoveTreeNode() {}
    shared_ptr<Move> m_move; // Key (public for now for debugging)
    vector<shared_ptr<MoveTreeNode>> m_children; // To to go to next move
};
