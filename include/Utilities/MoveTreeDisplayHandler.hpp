#pragma once

#include "MoveTree.hpp"
#include <optional>

struct MoveInfo 
{
    std::string m_content = "";
    int m_row = 0; 
    int m_indentLevel = 0;
    bool m_isInlineSubVariation = false;
    Move* m_movePtr = nullptr;
    std::optional<std::string> m_letterPrefix = std::nullopt;
};

class MoveTreeDisplayHandler 
{
public:
    MoveTreeDisplayHandler(MoveTree& tree);

    std::vector<MoveInfo> generateMoveInfo();

private:
    MoveTree& m_tree;
    std::vector<MoveInfo> m_moveInfos;

    void processNodeRec(MoveTree::Iterator& iter_, int level_, int& row_, bool isNewLineSubvariation_ = false);
    void processNode(MoveTree::Iterator& iter_, int level_, int& row_, bool isNewLineSubvariation_ = false);
};
