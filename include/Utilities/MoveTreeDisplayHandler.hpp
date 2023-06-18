#pragma once

#include "MoveTree.hpp"

struct MoveInfo {
    int m_row; // The line number on the panel
    std::string m_content; // The move itself, ex: 1.e4 or 2...d5
    bool m_isSubVariation; // If the move is a subvariation
    bool m_isSubSubVariation; // If the move is a sub-subvariation
    int m_subVariationIndex; // If it's a subvariation or sub-subvariation, its index (A,B,C... = 1,2,3...)
    bool m_isInlineSubVariation; // If the subvariation should be inline
    int m_level; // The depth level of the variation (0 for main line, 1 for sub variation, 2 for sub-sub variation, etc.)
};

class MoveTreeDisplayHandler {
public:
    MoveTreeDisplayHandler(MoveTree& tree);

    std::vector<MoveInfo> generateMoveInfo();

private:
    MoveTree& m_tree;
    std::vector<MoveInfo> m_moveInfos;

    void processNode(MoveTree::Iterator& iter_, int& currentRow_, int level_);
};
