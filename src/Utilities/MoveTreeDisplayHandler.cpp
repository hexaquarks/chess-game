#include "../../include/Utilities/MoveTreeDisplayHandler.hpp"

#include <algorithm>

namespace 
{
    bool shouldContinueTraversingUp(MoveTree::Iterator& iter)
    {
        return !iter.isAtTheBeginning() && 
                iter.getNbOfNodesAtCurrentLevel() == 1;
    }

    bool checkLineWrap(MoveTree::Iterator& iter)
    {
        MoveTree::Iterator it_temp = iter;
        it_temp.goToParent();
        return it_temp.getNbOfNodesAtCurrentLevel() != 1 && 
               it_temp.getNodeIdxAmongSiblings() == 0;
    }

    std::string getLetterPrefixNew(
        MoveTree::Iterator iter_, 
        int indentationLevel_) 
    {
        std::vector<int> indices;
        
        // Traverse the tree upwards to collect indices
        bool isLineWrap = false;
        while (!iter_.isAtTheBeginning() && !isLineWrap) 
        {
            indices.push_back(iter_.getNodeIdxAmongSiblings());
            iter_.goToParent();

            while (shouldContinueTraversingUp(iter_)) 
            {
                isLineWrap = checkLineWrap(iter_);
                if (isLineWrap) break;
                iter_.goToParent();
            }

        }

        std::reverse(indices.begin(), indices.end()); 
        
        // Construct the letter and number prefix
        std::stringstream prefix_ss;
        if (indices.size() == 1) 
        {
            char letter = 'A' + indices[0] - 1;
            prefix_ss << letter;
        }
        else 
        {
            for (int i = 0; i < indices.size(); ++i) 
            {
                if (i == 0) {
                    // The first letter
                    char letter = 'A' + indices[i] - 1;
                    prefix_ss << letter;
                } else {
                    // Subsequent numbers
                    if (i != 0) { 
                        if (i == 1) prefix_ss << indices[i];
                        else {
                            prefix_ss << "," << indices[i];
                        }
                    }
                }
            }
        }
        prefix_ss << ")";
        return prefix_ss.str();
    }
}

MoveTreeDisplayHandler::MoveTreeDisplayHandler(const MoveTree& tree_) 
: m_tree(tree_) 
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
    const int nodeDepth = iter_.getNodeLevel();
    info.m_content = parseMove(*move, nodeDepth / 2 + 1, nodeDepth % 2 != 0, isNewLineSubvariation_);
    info.m_indentLevel = level_;
    info.m_row = row_;
    info.m_movePtr = move;

    if (isNewLineSubvariation_)
    {
        info.m_letterPrefix = getLetterPrefixNew(iter_, level_);
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
        MoveTree::Iterator temp_iter = iter_; 
        if (temp_iter.goToGrandChild(0))
        {
            ++row_;
            processNodeRec(temp_iter, level_, row_);
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
    
    //printMoves(m_moveInfos);
    return m_moveInfos;
}

std::string printMoveInfos(const std::vector<MoveInfo>& moveInfos_, bool printToConsole) 
{
    std::ostringstream oss;
    int currentRow = -1; 
    bool isFirstMoveInSubvariation = true;

    oss << "===== Printing the generated moves info =====" << std::endl;
    
    for (const auto& info : moveInfos_) 
    {
        if (info.m_row != currentRow) 
        {
            if (currentRow != -1) oss << std::endl;
            currentRow = info.m_row;
            isFirstMoveInSubvariation = true;
        }

        if (isFirstMoveInSubvariation) 
        {
            for (int i = 0; i < 4 * (info.m_indentLevel - info.m_letterPrefix.has_value()); ++i) 
            {
                oss << " ";
            }
        } else oss << " ";

        if (info.m_letterPrefix.has_value()) 
        {
            oss << "    " << info.m_letterPrefix.value() << " ";
        }
        isFirstMoveInSubvariation = false;

        oss << info.m_content;
    }

    std::string result = oss.str();
    if (printToConsole) std::cout << result;

    return result;
}


std::string printMoveInfosGet(const std::vector<MoveInfo>& moveInfos_)
{
    if (moveInfos_.size() == 0) return "";
    return printMoveInfos(moveInfos_, false);
}
