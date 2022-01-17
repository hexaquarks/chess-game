#include "../include/MoveList.hpp"

void MoveList::insertNode(moveType move) const {
    MoveNode* newNode = new MoveNode(move);
  
    if (m_head == NULL) {
        // m_head = newNode;
        return;
    }
  
    // Traverse list
    MoveNode* MoveNode = m_head;
    while (MoveNode->m_next != NULL) {
        MoveNode = MoveNode->m_next;
    }

    MoveNode->m_next = newNode;
}
