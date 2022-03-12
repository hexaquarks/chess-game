#include "../../include/Utilities/MoveTree.hpp"

MoveTreeNode* MoveTree::findNode(MoveTreeNode*& root) {
    if (root == currNode) {
        return root;
    } else {
        for (auto child: root->m_children) {
            MoveTreeNode* result = findNode(child);
            if (result != nullptr) return result;
        }
    }
    return nullptr;
}

void MoveTree::insertNode(Move& newMove) {
    MoveTreeNode* newNode = new MoveTreeNode{newMove}; // Make new node with the move

    if (!m_root) {
        // currNode is head
        m_root = newNode;
        m_root->m_parent = nullptr;  
    } else {
        // currNode is a node in the body
        // m_root->m_children.emplace_back(newMove);
        // newNode->m_parent = m_root;

        MoveTreeNode* temp = findNode(currNode);
        // cout << "parent of curr node is  : " <<  currNode->m_parent->m_move.getTarget().first << endl;
        temp->m_children.emplace_back(newNode); // assign new node as children to currNode
        newNode->m_parent = temp; // set currNode to be new Node's parent
        
    }
    ++numberOfMoves;
    MoveTreeNode* newCurrNode = newNode; // traverse currNode to the new node
    currNode = newCurrNode;

}

void MoveTree::goToNextNode(int slectedMoveIndex) {
    // go to next move only if it is not a Leaf node
    if (currNode->m_children.size() != 0)
        currNode = currNode->m_children.at(slectedMoveIndex);
}

void MoveTree::goToPreviousNode() {   
    // go to previous move only if it is not NULL
    if (currNode->m_parent != nullptr)
        currNode = currNode->m_parent;
}

void printTreeRec(MoveTreeNode*& root, vector<bool> flag,
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
    cout << "===== Printing the move tree =====";
    // printTreeRec(m_root, flag);
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
