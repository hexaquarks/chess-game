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

    struct Iterator {
        using iterator_category = bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        Iterator(MoveTreeNode* ptr): m_ptr(ptr) {};

        MoveTreeNode& operator*() const { return *m_ptr; }
        MoveTreeNode* operator->() { return m_ptr; }
        MoveTreeNode* get() { return m_ptr; }

        void goToChild(int i) { m_ptr = m_ptr->m_children.at(i); }

        void addChild(MoveTreeNode* child) {
            if (m_ptr == nullptr) {
                m_ptr = child;
            } else {
                m_ptr->m_children.emplace_back(child);
                child->m_parent = m_ptr;
                ++(m_ptr->childNumber);
                goToChild(m_ptr->childNumber-1);
            }
        }

        void goToParent() { m_ptr = m_ptr->m_parent; }

        // Prefix increment
        MoveTreeNode* operator++() {
            goToChild(0); return m_ptr;
        }

        // Postfix increment
        MoveTreeNode* operator++(int) { 
            MoveTreeNode* res = m_ptr; goToChild(0); return res;
        }

        // Shift child number
        MoveTreeNode* operator>>(int n) {
            int childNumber = m_ptr->childNumber;
            goToParent();
            goToChild(childNumber+n);
            return m_ptr;
        }

        // Shift child number
        MoveTreeNode* operator<<(int n) {
            int childNumber = m_ptr->childNumber;
            goToParent();
            goToChild(childNumber-n);
            return m_ptr;
        }

        // Prefix decrement
        MoveTreeNode* operator--() { goToParent(); return m_ptr; }

        // Postfix decrement
        MoveTreeNode* operator--(int) { MoveTreeNode* res = m_ptr; goToParent(); return res; }

        friend bool operator ==(const Iterator& a, const Iterator& b) 
            { return a.m_ptr == b.m_ptr; };
        friend bool operator !=(const Iterator& a, const Iterator& b) 
            { return a.m_ptr != b.m_ptr; };

        private:
        MoveTreeNode* m_ptr;
    };

    Iterator begin() { return Iterator(m_root); }
    Iterator end()   { return Iterator(nullptr); } 

    void insertNode(Move&, MoveTree::Iterator&);
    void goToNextNode(int, MoveTree::Iterator&);
    void goToPreviousNode(MoveTree::Iterator&);
    void printTree();
    void printTreeRec(MoveTreeNode*&, vector<bool>, int a = 0, bool b = false);
    int getNumberOfMoves() { return numberOfMoves; }
    void printPreorder(MoveTreeNode*&);
};