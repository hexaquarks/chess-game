#include "../../include/Utilities/MoveTreeDisplayHandler.hpp"

MoveTreeDisplayHandler::MoveTreeDisplayHandler(MoveTree& tree) : m_tree(tree) {}

void printMoves(std::vector<MoveInfo>& moveInfo_) {
    std::cout << "There are currently " << moveInfo_.size() << " moves to display: ";
    for (const MoveInfo& move : moveInfo_) {
        std::cout << move.m_content << ",";
    }
    std::cout << std::endl;
}

void MoveTreeDisplayHandler::processNode(MoveTree::Iterator& iter_, int& currentRow_, int level_) {
    auto* move = iter_->m_move.get();
    if (move == nullptr) return;

    MoveInfo info;
    info.m_row = currentRow_++;
    info.m_content = parseMove(*move, (level_ / 2) + 1, level_ % 2 != 0);
    info.m_isSubVariation = level_ > 0;
    info.m_isSubSubVariation = level_ > 1;
    info.m_subVariationIndex = iter_->childNumber;
    info.m_isInlineSubVariation = iter_->m_children.size() == 1;
    info.m_level = level_;

    m_moveInfos.push_back(info);

    for (size_t i = 0; i < iter_->m_children.size(); ++i) {
        iter_.goToChild(i);  
        processNode(iter_, currentRow_, level_ + 1); 
        iter_.goToParent();  
    }
}

std::vector<MoveInfo> MoveTreeDisplayHandler::generateMoveInfo() {
    // No information has been generated yet, display nothing.
    if (m_tree.getNumberOfMoves() == 0) return {};

    m_moveInfos.clear();
    int currentRow = 0;

    MoveTree::Iterator iter_ = m_tree.begin();

    // Note The first node is nullptr as per our API, so we don't process It.
    for (size_t i = 0; i < iter_->m_children.size(); ++i)
    {
        iter_.goToChild(i);
        processNode(iter_, currentRow, 0);
        iter_.goToParent();
    }
    
    return m_moveInfos;
}
