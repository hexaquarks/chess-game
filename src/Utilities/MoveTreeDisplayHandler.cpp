#include "../../include/Utilities/MoveTreeDisplayHandler.hpp"

namespace 
{
    std::string getLetterPrefix(MoveTree::Iterator iter_, int indentationLevel_)
    {
        // Base case
        if(indentationLevel_ == 1) 
        {
            return std::string(1, 'A' + iter_.getNodeIdxAmongSiblings() - 1) + ")";
        }
        else 
        {
            auto currentNode = iter_.get();

            iter_.goToParent();
            std::string parentPrefix = getLetterPrefix(iter_, indentationLevel_ - 1);

            // Restore iterator to current node
            iter_.get() = currentNode;

            return parentPrefix + std::to_string(iter_.getNodeIdxAmongSiblings() + 1) + ")";
        }
    }
}

MoveTreeDisplayHandler::MoveTreeDisplayHandler(MoveTree& tree) 
: m_tree(tree) 
{
}

void printMoves(std::vector<MoveInfo>& moveInfo_) 
{
    std::cout << "There are currently " << moveInfo_.size() << std::endl; 
    std::cout << "Moves to display: " << std::endl;
    for (const MoveInfo& move : moveInfo_) {
        std::cout << move.m_content << ", row : " << move.m_row << ", depth: " << move.m_indentLevel << endl;;
    }
    std::cout << std::endl;
}

void MoveTreeDisplayHandler::processNode(
    MoveTree::Iterator& iter_, 
    int level_, 
    int& row_, 
    bool isNewLineSubvariation_) 
{
    auto* move = iter_->m_move.get();
    if (move == nullptr) return;

    MoveInfo info;
    int nodeDepth = iter_.getNodeLevel();
    info.m_content = parseMove(*move, nodeDepth / 2 + 1, nodeDepth % 2 != 0, isNewLineSubvariation_);
    info.m_indentLevel = level_;
    info.m_row = row_;

    if (isNewLineSubvariation_)
    {
        std::cout <<"in" << std::endl;
        info.m_letterPrefix = getLetterPrefix(iter_, level_);
    }


    m_moveInfos.push_back(info);
}
void MoveTreeDisplayHandler::processNodeRec(
    MoveTree::Iterator& iter_, 
    int level_, 
    int& row_, 
    bool isNewLineSubvariation_) 
{
    processNode(iter_, level_, row_, isNewLineSubvariation_);
    // If only 1 child go as usual
    // If more than one child, need to start with non-main line

    if (iter_->m_children.size() < 2)
    {
        if (iter_->m_children.size() == 0) return;
        iter_.goToChild(0);
        processNodeRec(iter_, level_, row_); 
        iter_.goToParent();
    }
    else
    {
        // process index 0.
        iter_.goToChild(0);
        processNode(iter_, level_, row_);
        iter_.goToParent();
        
        // then loop over index 1 to N-1
        for (size_t i = 1; i < iter_->m_children.size(); ++i) {
            ++row_;
            iter_.goToChild(i);
            processNodeRec(iter_, level_ + 1, row_, true); 
            iter_.goToParent();
        }

        // after the loop, goToGrandChild(0) and increase row if 
        // main line stretches after.
        if (iter_.goToGrandChild(0))
        {
            ++row_;
            processNodeRec(iter_, level_, row_);
        }
    }
}


std::vector<MoveInfo> MoveTreeDisplayHandler::generateMoveInfo() 
{
    if (m_tree.getNumberOfMoves() == 0) return {};

    m_moveInfos.clear();

    MoveTree::Iterator iter = m_tree.begin();
    int row = 0;

    for (size_t i = 0; i < iter->m_children.size(); ++i)
    {
        iter.goToChild(i);
        processNodeRec(iter, 0, row);
        iter.goToParent();
    }
    
    printMoves(m_moveInfos);
    return m_moveInfos;
}
