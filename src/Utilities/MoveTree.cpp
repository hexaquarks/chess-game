#include "../../include/Utilities/MoveTree.hpp"

MoveTreeNode* MoveTree::findNode(MoveTreeNode*& node_) {
    for (auto child: m_root->m_children) {
        MoveTreeNode* result = findNode(child);
        if (result != nullptr) return result;
    }
    return nullptr;
}

void MoveTree::insertNode(Move& newMove_, MoveTree::Iterator& it_) {
    for (int i = 0; i < it_->m_children.size(); ++i) {
        if (*(it_->m_children[i]->m_move.get()) == newMove_) {
            it_.goToChild(i);
            return;
        }
    }
    MoveTreeNode* newNode = new MoveTreeNode(newMove_); // Make new node with the move
    it_.addChild(newNode);
    ++numberOfMoves;
}

void MoveTree::goToNextNode(const int slectedMoveIndex_, MoveTree::Iterator& it_) {
    // go to next move only if it is not a Leaf node
    if (it_->m_children.size() != 0)
        it_.goToChild(slectedMoveIndex_);
}

void MoveTree::goToPreviousNode(MoveTree::Iterator& it_) {   
    // go to previous move only if it is not NULL
    if (it_->m_parent != nullptr) --it_;
}

void MoveTree::printTreeRec(MoveTreeNode* root_, vector<bool> flag_, int depth_, bool isLast_) {
    if (root_->m_move.get() == nullptr) return;

    for (int i = 1; i < depth_; ++i) {
        if (flag_[i] == true) cout << "| " << " " << " " << " ";
        else  cout << " " << " " << " " << " ";
    }

    coor2d tar = root_->m_move->getTarget();
    if (depth_ == 0)
        cout << "(" << tar.first << "," << tar.second << ")" << '\n';
    else if (isLast_) {
        cout << "+--- " << "(" << tar.first << "," << tar.second << ")" << '\n';
        flag_[depth_] = false;
    } else 
        cout << "+--- " << "(" << tar.first << "," << tar.second << ")" << '\n';

    int it = 0;
    for (auto i = root_->m_children.begin(); i != root_->m_children.end(); ++i, ++it)
        printTreeRec(*i, flag_, depth_ + 1,it == (root_->m_children.size()) - 1);

    flag_[depth_] = true;   
}

void MoveTree::printTree() {
    vector<bool> flag(getNumberOfMoves(), true);
    cout << "===== Printing the move tree =====" << endl;
    MoveTreeNode* movePtr = m_root.get()->m_children.at(0);
    printTreeRec(movePtr, flag);
}


void MoveTree::printPreorder(MoveTreeNode*& m_root) {
    if (m_root == nullptr) return;

    int i = 0;
    MoveTreeNode *temp = nullptr;
    coor2d tar = m_root->m_move->getTarget();
    cout << "  " << "(" << tar.first << "," << tar.second << ")";
    // iterating the child of given node
    while (i < m_root->m_children.size()) {
        temp = m_root->m_children.at(i);
        this->printPreorder(temp);
        i++;
    }
}

int MoveTree::getNodeLevel(MoveTree::Iterator& it_) {
    MoveTreeNode* temp = it_.get();
    int i = 0;
    while(temp->m_parent != nullptr) {
        temp = temp->m_parent;
        ++i;
    }
    return i;
}
