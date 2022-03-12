#pragma once
#include "Move.hpp"
#include "MoveTreeNode.hpp"

using namespace std;

class MoveTree {
    public: 
    MoveTreeNode* m_root = nullptr; // Root of the tree
    int numberOfMoves = 0;

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

    struct Iterator {
        using iterator_category = bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        Iterator(MoveTreeNode* ptr): m_ptr(ptr) {};

        MoveTreeNode& operator*() const { return *m_ptr; }
        MoveTreeNode* operator->() { return m_ptr; }

        // Prefix increment
        // Iterator& operator++(int i) { m_ptr = m_ptr->m_children.at(i); return *this; }  

        // Postfix increment
        Iterator operator++(int i) { 
            Iterator tmp = m_ptr; m_ptr = m_ptr->m_children.at(i); return tmp; 
        }

        // Prefix decrement
        Iterator& operator--() { m_ptr = m_ptr->m_parent; return *this; }  

        // Postfix decrement
        Iterator operator++(int) { Iterator tmp = m_ptr; m_ptr = m_ptr->m_parent; return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) 
            { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) 
            { return a.m_ptr != b.m_ptr; };

        private:
        MoveTreeNode* m_ptr;
    };
    Iterator begin() { return Iterator(m_root); }
    Iterator end()   { return Iterator(nullptr); } 
};

inline MoveTreeNode* currNode = nullptr;  // Necessary to declare after class