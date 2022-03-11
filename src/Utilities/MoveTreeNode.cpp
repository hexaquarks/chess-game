#include "../../include/Utilities/MoveTreeNode.hpp"

MoveTreeNode::MoveTreeNode(Move& move): m_move(move) { };

void MoveTreeNode::insertNode(MoveTreeNode* currNode, Move& newMove) {
    MoveTreeNode* newNode = new MoveTreeNode{newMove}; // Make new node with the move

    if (currNode == nullptr) {
        // currNode is head
        currNode = newNode;
        currNode->m_parent = nullptr;
    } else {
        // currNode is a node in the body
        currNode->m_children.emplace_back(newNode); // assign new node as children to currNode
        newNode->m_parent = currNode; // set currNode to be new Node's parent
        currNode = currNode->m_children.back(); // traverse currNode to the new node
    }
}

void MoveTreeNode::goToNextNode(int slectedMoveIndex) {
    currNode = currNode->m_children.at(slectedMoveIndex);
    // for(auto node: currNode->m_children) {
    //     if (node->m_move == selectedMove) {
    //         // need to make operator comparison.. too complicated for nothing. Get index instead
    //     }
    // }
}

void MoveTreeNode::goToPreviousNode(int selectedMoveIndex) {   
    currNode = currNode->m_parent;
}