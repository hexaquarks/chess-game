#include "../../include/Utilities/MoveTree.hpp"


void MoveTree::insertNode(const shared_ptr<Move>& newMove_, MoveTree::Iterator& it_)
{
    for (int i = 0; i < it_->m_children.size(); ++i)
    {
        if (*it_->m_children[i]->m_move == *newMove_)
        {
            it_.goToChild(i);
            return;
        }
    }
    shared_ptr<MoveTreeNode> newNode = make_shared<MoveTreeNode>(newMove_); // Make new node with the move
    it_.addChild(newNode);
    ++numberOfMoves;
}

void MoveTree::goToNextNode(int slectedMoveIndex_, MoveTree::Iterator& it_)
{
    // Go to next move only if it is not a Leaf node
    if (!it_->m_children.empty())
        it_.goToChild(slectedMoveIndex_);
}

void MoveTree::goToPreviousNode(MoveTree::Iterator& it_)
{
    // Go to previous move only if it is not NULL
    if (it_->m_parent) --it_;
}

void MoveTree::printTreeRec(shared_ptr<MoveTreeNode>& root_, vector<bool> flag_, int depth_, bool isLast_)
{
    if (!root_->m_move) return;

    for (int i = 1; i < depth_; ++i)
    {
        if (flag_[i]) cout << "| " << " " << " " << " ";
        else cout << " " << " " << " " << " ";
    }

    coor2d tar = root_->m_move->getTarget();
    if (depth_ == 0)
        cout << "(" << tar.first << "," << tar.second << ")" << '\n';
    else if (isLast_)
    {
        cout << "+--- " << "(" << tar.first << "," << tar.second << ")" << '\n';
        flag_[depth_] = false;
    }
    else
        cout << "+--- " << "(" << tar.first << "," << tar.second << ")" << '\n';

    int it = 0;
    for (auto i = root_->m_children.begin(); i != root_->m_children.end(); ++i, ++it)
        printTreeRec(*i, flag_, depth_ + 1, it == (root_->m_children.size()) - 1);

    flag_[depth_] = true;
}

void MoveTree::printTree()
{
    vector<bool> flag(getNumberOfMoves(), true);
    cout << "===== Printing the move tree =====" << endl;
    shared_ptr<MoveTreeNode> movePtr = m_root->m_children.at(0);
    printTreeRec(movePtr, flag);
}


void MoveTree::printPreorder(shared_ptr<MoveTreeNode>& m_root)
{
    if (!m_root) return;

    int i = 0;
    shared_ptr<MoveTreeNode> temp;
    coor2d tar = m_root->m_move->getTarget();
    cout << " " << "(" << tar.first << "," << tar.second << ")";
    // Iterating the child of given node
    while (i < m_root->m_children.size())
    {
        temp = m_root->m_children.at(i);
        this->printPreorder(temp);
        i++;
    }
}

int MoveTree::getNodeLevel(MoveTree::Iterator& it_)
{
    shared_ptr<MoveTreeNode> temp = it_.get();
    int i = 0;
    while (temp->m_parent)
    {
        temp = temp->m_parent;
        ++i;
    }
    return i;
}
