#pragma once
#include "Move.hpp"
#include "MoveTreeNode.hpp"

using namespace std;

class MoveTree {
    MoveTreeNode* m_root = nullptr; // Root of the tree
    int numberOfMoves = 0;

    public: 
    MoveTreeNode* findNode(MoveTreeNode*&);
    MoveTreeNode* getNode();
    MoveTreeNode* getRoot() { return m_root; }

    void insertNode(Move&);
    void goToNextNode(int);
    void goToPreviousNode();
    void printTreeRec(MoveTreeNode*&, vector<bool>, int a = 0, bool b = false);
    void printTree();
    int getNumberOfMoves() { return numberOfMoves; }
    void printPreorder(MoveTreeNode*&);
};

inline MoveTreeNode* currNode = nullptr;  // Necessary to declare after class