#pragma once
#include "Move.hpp"
#include "MoveTreeNode.hpp"

using namespace std;

class MoveTree
{
    public:
    shared_ptr<MoveTreeNode> m_root = make_shared<MoveTreeNode>(); // Root of the tree
    int numberOfMoves = 0;

    shared_ptr<MoveTreeNode>& getRoot() { return m_root; }

    class Iterator
    {
        shared_ptr<MoveTreeNode> m_ptr;

        public:
        using iterator_category = bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;

        Iterator(shared_ptr<MoveTreeNode>& ptr): m_ptr(ptr)
        {
        }

        Iterator()
        {
        }

        MoveTreeNode& operator*() const { return *m_ptr; }
        shared_ptr<MoveTreeNode>& operator->() { return m_ptr; }
        shared_ptr<MoveTreeNode>& get() { return m_ptr; }

        void goToChild(int i) { m_ptr = m_ptr->m_children.at(i); }

        void addChild(shared_ptr<MoveTreeNode>& child)
        {
            if (!m_ptr)
            {
                m_ptr = child;
            }
            else
            {
                m_ptr->m_children.emplace_back(child);
                child->m_parent = m_ptr;
                ++(m_ptr->childNumber);
                goToChild(m_ptr->childNumber-1);
            }
        }

        void goToParent() { m_ptr = m_ptr->m_parent; }

        int getNodeLevel() {
            shared_ptr<MoveTreeNode> temp = m_ptr;
            int i = 0;
            while (temp->m_parent)
            {
                temp = temp->m_parent;
                ++i;
            }
            return i;
        }

        // Prefix increment
        shared_ptr<MoveTreeNode> operator++()
        {
            goToChild(0); return m_ptr;
        }

        // Postfix increment
        shared_ptr<MoveTreeNode> operator++(int)
        {
            shared_ptr<MoveTreeNode> res = m_ptr; goToChild(0); return res;
        }

        // Shift child number
        shared_ptr<MoveTreeNode> operator>>(int n)
        {
            int childNumber = m_ptr->childNumber;
            goToParent();
            goToChild(childNumber+n);
            return m_ptr;
        }

        // Shift child number
        shared_ptr<MoveTreeNode> operator<<(int n)
        {
            int childNumber = m_ptr->childNumber;
            goToParent();
            goToChild(childNumber-n);
            return m_ptr;
        }

        // Prefix decrement
        shared_ptr<MoveTreeNode> operator--() { goToParent(); return m_ptr; }

        // Postfix decrement
        shared_ptr<MoveTreeNode> operator--(int) { shared_ptr<MoveTreeNode> res = m_ptr; goToParent(); return res; }

        friend bool operator ==(const Iterator& a, const Iterator& b) {
            return a.m_ptr == b.m_ptr;
        };
        friend bool operator !=(const Iterator& a, const Iterator& b) {
            return a.m_ptr != b.m_ptr;
        };
    };

    Iterator begin() { return Iterator(getRoot()); }
    Iterator end() { return Iterator(); }

    void insertNode(shared_ptr<Move>&, MoveTree::Iterator&);
    void goToNextNode(const int, MoveTree::Iterator&);
    void goToPreviousNode(MoveTree::Iterator&);
    void printTree();
    void printTreeRec(shared_ptr<MoveTreeNode>&, vector<bool>, int a = 0, bool b = false);
    int getNumberOfMoves() { return numberOfMoves; }
    void printPreorder(shared_ptr<MoveTreeNode>&);
    int getNodeLevel(MoveTree::Iterator&);
};
