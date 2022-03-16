#include "../../include/Utilities/MoveTreeNode.hpp"

MoveTreeNode::MoveTreeNode(Move& move): m_move(make_unique<Move>(move)) {}
