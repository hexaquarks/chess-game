#pragma once
#include "MoveTreeNode.hpp"

using namespace std;
#include <iostream>
#include <sstream>

class MoveTree
{
public:
    shared_ptr<MoveTreeNode> m_root = make_shared<MoveTreeNode>(); // Root of the tree
    int numberOfMoves = 0;

    shared_ptr<MoveTreeNode> getRoot() const { return m_root; }

    class Iterator
    {
        shared_ptr<MoveTreeNode> m_ptr;

    public:
        using iterator_category = bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;

        explicit Iterator(const shared_ptr<MoveTreeNode>& ptr): m_ptr(ptr) {}
        Iterator() = default;

        MoveTreeNode& operator*() const { return *m_ptr; }
        shared_ptr<MoveTreeNode>& operator->() { return m_ptr; }
        shared_ptr<MoveTreeNode>& get() { return m_ptr; }

        bool isAtTheBeginning() const 
        {
            return !m_ptr->m_parent;
        }

        bool isAtTheEnd() const 
        {
            return m_ptr->m_children.empty();
        }

        void reset() 
        {
            while (!isAtTheBeginning()) m_ptr = m_ptr->m_parent;
        }

        bool currentNodeHasMoreThanOneVariation()
        {
            return m_ptr->m_children.size() > 1;
        }

        const shared_ptr<MoveTreeNode>& getCurrentNode()
        {
            return m_ptr;
        }

        void goToChild(int i) { m_ptr = m_ptr->m_children.at(i); }

        void addChild(shared_ptr<MoveTreeNode>& child)
        {
            if (!m_ptr)
            {
                m_ptr = child;
            }
            else
            {
                m_ptr->m_children.push_back(child);
                child->m_parent = m_ptr;
                ++(m_ptr->childNumber);
                goToChild(m_ptr->childNumber-1);
            }
        }

        bool goToParent() 
        {
            shared_ptr<MoveTreeNode> parent = m_ptr->m_parent;
            if (parent) {
                m_ptr = parent;
                return true;
            }
            return false;
        }

        bool goToGrandChild(int i)
        {
            auto temp = m_ptr;   
            if (temp->m_children.size() <= i) return false;
            temp = temp->m_children.at(i);
            if (temp->m_children.size() <= i) return false;
            temp = temp->m_children.at(i);

            m_ptr = temp;
            return true;
        }

        int getNodeLevel() 
        {
            shared_ptr<MoveTreeNode> temp = m_ptr;
            int i = 0;
            while (temp->m_parent)
            {
                temp = temp->m_parent;
                ++i;
            }
            return i;
        }

        int getNodeIdxAmongSiblings() 
        {
            auto temp = m_ptr;
            if (!temp) return -1;

            auto parent = temp->m_parent;
            
            if (!parent) return -1;
            
            for (size_t i = 0; i < parent->m_children.size(); ++i) 
            {
                if (parent->m_children.at(i) == temp) return i;
            }
            
            // Index not found
            return -1;
        }

        int getNbOfNodesAtCurrentLevel()
        {
            if (!m_ptr || !m_ptr->m_parent) return 1;
            return m_ptr->m_parent->m_children.size();
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
            if (!m_ptr) return m_ptr; // we're at the root - no siblings here
            int childNumber = m_ptr->childNumber;
            goToChild(childNumber+n);
            return m_ptr;
        }

        // Shift child number
        shared_ptr<MoveTreeNode> operator<<(int n)
        {
            if (!m_ptr) return m_ptr; // we're at the root - no siblings here
            int childNumber = m_ptr->childNumber;
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

    Iterator begin() const { return Iterator(getRoot()); }

    void insertNode(const shared_ptr<Move>&, MoveTree::Iterator&);
    void goToNextNode(int, MoveTree::Iterator&);
    void goToPreviousNode(MoveTree::Iterator&);
    void printTree(std::ostream& os_ = std::cout) const;
    std::string printTreeGet() const;
    void printTreeRec(shared_ptr<MoveTreeNode>&, vector<bool>, std::ostream& os_, int a = 0, bool b = false) const;
    int getNumberOfMoves() const { return numberOfMoves; }
    void printPreorder(shared_ptr<MoveTreeNode>&);
    int getNodeLevel(MoveTree::Iterator&);
    void clear() {
        m_root = make_shared<MoveTreeNode>();
        numberOfMoves = 0;
    }
};
