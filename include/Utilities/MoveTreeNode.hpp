#pragma once
#include "Move.hpp"

using namespace std;

class MoveTreeNode {
    Move& m_move; // Key
    MoveTreeNode* m_parent; // To go to previous move
    vector<MoveTreeNode*> m_children;   // To to go to next move

    public: 
    MoveTreeNode(Move&);

    void insertNode(MoveTreeNode*, Move&);
    void goToNextNode(int);
    void goToPreviousNode(int);
    MoveTreeNode* getNode();

};

static inline MoveTreeNode* currNode = nullptr;  // Necessary to declare after class