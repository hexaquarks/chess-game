#include "../../include/Utilities/MoveTreeNode.hpp"

MoveTreeNode::MoveTreeNode(const Move& move_): m_move(make_unique<Move>(move_)) {}
