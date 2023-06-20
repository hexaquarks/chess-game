#pragma once

#include "MoveTree.hpp"

struct MoveInfo {
    std::string m_content;
    int m_row; 
    int m_indentLevel;
    bool m_isInlineSubVariation;
};


class MoveTreeDisplayHandler {
public:
    MoveTreeDisplayHandler(MoveTree& tree);

    std::vector<MoveInfo> generateMoveInfo();

private:
    MoveTree& m_tree;
    std::vector<MoveInfo> m_moveInfos;

    void processNodeRec(MoveTree::Iterator& iter_, int level_, int& row_);
    void processNode(MoveTree::Iterator& iter_, int level_, int& row_);
};
