#include "../../include/Utilities/MoveTree.hpp"

MoveTreeNode* MoveTree::findNode(MoveTreeNode*& node) {
    if (m_root == node) {
        return m_root;
    } else {
        for (auto child: m_root->m_children) {
            MoveTreeNode* result = findNode(child);
            if (result != nullptr) return result;
        }
    }
    return nullptr;
}

void MoveTree::insertNode(Move& newMove, MoveTree::Iterator& it) {
    MoveTreeNode* newNode = new MoveTreeNode(newMove); // Make new node with the move
    it.addChild(newNode);

    if (m_root == nullptr) m_root = newNode;
    ++numberOfMoves;
}

void MoveTree::goToNextNode(int slectedMoveIndex, MoveTree::Iterator& it) {
    // go to next move only if it is not a Leaf node
    if (it->m_children.size() != 0)
        it.goToChild(slectedMoveIndex);
}

void MoveTree::goToPreviousNode(MoveTree::Iterator& it) {   
    // go to previous move only if it is not NULL
    if (it->m_parent != nullptr) --it;
}

void MoveTree::printTreeRec(MoveTreeNode*& root, vector<bool> flag,
    int depth, bool isLast) {

    if (root == nullptr) return;

    for (int i = 1; i < depth; ++i) {
        if (flag[i] == true) cout << "| " << " " << " " << " ";
        else  cout << " " << " " << " " << " ";
    }
    coor2d tar = root->m_move.getTarget();
    if (depth == 0)
        cout << "(" << tar.first << "," << tar.second << ")" << '\n';
    else if (isLast) {
        cout << "+--- " << "(" << tar.first << "," << tar.second << ")" << '\n';
        flag[depth] = false;
    } else 
        cout << "+--- " << "(" << tar.first << "," << tar.second << ")" << '\n';

    int it = 0;
    for (auto i = root->m_children.begin(); i != root->m_children.end(); ++i, ++it)
        printTreeRec(*i, flag, depth + 1,it == (root->m_children.size()) - 1);

    flag[depth] = true;   
}

void MoveTree::printTree() {
    vector<bool> flag(getNumberOfMoves(), true);
    cout << "===== Printing the move tree =====" << endl;
    printTreeRec(m_root, flag);
}


void MoveTree::printPreorder(MoveTreeNode*& m_root) {
    if (m_root == nullptr) return;

    int i = 0;
    MoveTreeNode *temp = nullptr;
    coor2d tar = m_root->m_move.getTarget();
    cout << "  " << "(" << tar.first << "," << tar.second << ")";
    // iterating the child of given node
    while (i < m_root->m_children.size()) {
        temp = m_root->m_children.at(i);
        this->printPreorder(temp);
        i++;
    }
}

int MoveTree::getNodeLevel(MoveTree::Iterator& it) {
    MoveTreeNode* temp = it.get();
    int i = 0;
    while(temp->m_parent != nullptr) {
        temp = temp->m_parent;
        ++i;
    }
    return i;
}
