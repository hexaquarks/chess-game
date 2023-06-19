#include "../../include/Utilities/MoveTreeDisplayHandler.hpp"

MoveTreeDisplayHandler::MoveTreeDisplayHandler(MoveTree& tree) : m_tree(tree) {}

void printMoves(std::vector<MoveInfo>& moveInfo_) {
    std::cout << "There are currently " << moveInfo_.size() << " moves to display: ";
    for (const MoveInfo& move : moveInfo_) {
        std::cout << move.m_content << ", (" << move.m_row << ")";
    }
    std::cout << std::endl;
}

void MoveTreeDisplayHandler::processNode(MoveTree::Iterator& iter_, int level_, int& row_) {
    auto* move = iter_->m_move.get();
    if (move == nullptr) return;

    MoveInfo info;
    info.m_content = parseMove(*move, level_ / 2 + 1, level_ % 2 == 0, level_ % 2 != 0);
    info.m_depth = level_;
    info.m_row = row_;

    m_moveInfos.push_back(info);

    for (size_t i = 0; i < iter_->m_children.size(); ++i) {
        if (i != 0) ++row_;
        iter_.goToChild(i);
        processNode(iter_, level_ + 1, row_); 
        iter_.goToParent();
    }
}


std::vector<MoveInfo> MoveTreeDisplayHandler::generateMoveInfo() {
    if (m_tree.getNumberOfMoves() == 0) return {};

    m_moveInfos.clear();

    MoveTree::Iterator iter = m_tree.begin();

    int row = 0;

    for (size_t i = 0; i < iter->m_children.size(); ++i)
    {
        iter.goToChild(i);
        processNode(iter, 0, row);
        iter.goToParent();
    }
    
    printMoves(m_moveInfos);
    return m_moveInfos;
}
