#include "../../include/Utilities/MoveTreeDisplayHandler.hpp"

MoveTreeDisplayHandler::MoveTreeDisplayHandler(MoveTree& tree) : m_tree(tree) {}

void MoveTreeDisplayHandler::processNode(MoveTree::Iterator& iter_, int& currentRow_, int level_) {
    MoveInfo info;
    info.m_row = currentRow_++;
    info.m_content = parseMove(*(iter_->m_move.get()), (level_ / 2) + 1, level_ % 2 != 0);
    info.m_isSubVariation = level_ > 0;
    info.m_isSubSubVariation = level_ > 1;
    info.m_subVariationIndex = iter_->childNumber;
    info.m_isInlineSubVariation = iter_->m_children.size() == 1;
    info.m_level = level_;

    m_moveInfos.push_back(info);

    if (iter_->m_children.empty())
        return;

    ++iter_;
    processNode(iter_, currentRow_, level_);
}

void MoveTreeDisplayHandler::processSubNodes(MoveTree::Iterator& iter_, int& currentRow_, int level_) {
    for (int i = 0; i < iter_->m_children.size(); ++i) {
        iter_ >> i;
        processNode(iter_, currentRow_, level_ + 1);
        iter_ << i;
    }
}

std::vector<MoveInfo> MoveTreeDisplayHandler::generateMoveInfo() {
    m_moveInfos.clear();
    int currentRow = 0;

    MoveTree::Iterator iter_ = m_tree.begin();

    processNode(iter_, currentRow, 0);
    processSubNodes(iter_, currentRow, 0);

    return m_moveInfos;
}
