#include "../../include/Utilities/MoveTreeNode.hpp"
#include "../../include/Utilities/MoveTree.hpp"

MoveTreeNode::MoveTreeNode(Move& move): m_move(make_unique<Move>(move)) { };

int MoveTreeNode::getNodeLevel() {
    MoveTreeNode* temp = this;
    int i = 0;
    while (temp->m_parent != nullptr) {
        temp = temp->m_parent;
        ++i;
    }
    return i;
}