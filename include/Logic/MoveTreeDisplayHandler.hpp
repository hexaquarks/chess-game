#pragma once

#include "MoveTree.hpp"
#include <optional>

struct MoveInfo 
{
    std::string m_content = "";
    int m_row = 0; 
    int m_indentLevel = 0;
    // bool m_isInlineSubVariation = false;  -- TODO in the future 
    Move* m_movePtr = nullptr;
    std::optional<std::string> m_letterPrefix = std::nullopt;
};

class MoveTreeDisplayHandler 
{
public:
    explicit MoveTreeDisplayHandler(const MoveTree& tree_);
    MoveTreeDisplayHandler() = default;

    std::vector<MoveInfo> generateMoveInfo();

private:
    const MoveTree& m_tree;
    std::vector<MoveInfo> m_moveInfos;

    void processNodeRec(MoveTree::Iterator& iter_, int level_, int& row_, bool isNewLineSubvariation_ = false);
    void processNode(MoveTree::Iterator& iter_, int level_, int& row_, bool isNewLineSubvariation_ = false);
};

std::string printMoveInfos(
    const std::vector<MoveInfo>& moveInfos_, 
    bool printToConsole = true);
std::string printMoveInfosGet(const std::vector<MoveInfo>& moveInfos_);
