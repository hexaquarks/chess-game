#pragma once
#include <memory>
#include "Move.hpp"

class MoveTreeNode
{
public:
    MoveTreeNode(const std::shared_ptr<Move>&); // Public for now for debbuging
    MoveTreeNode() {}

    int childNumber = 0;
    std::shared_ptr<MoveTreeNode> m_parent; // To go to previous move
    std::shared_ptr<Move> m_move; // Key (public for now for debugging)
    std::vector<std::shared_ptr<MoveTreeNode>> m_children; // To to go to next move

private:
    static inline int numberOfMoves = 0;
};
