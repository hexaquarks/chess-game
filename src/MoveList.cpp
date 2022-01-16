#include "../include/Piece.hpp"
#include <iostream>
  
struct MoveNode {
public:
    moveType m_move; 
    MoveNode* m_next;
  
    // Parameterised Constructor
    MoveNode(moveType move) {
        this->m_move = move;
        this->m_next = NULL;
    }
};
  
class MoveList {
    MoveNode* m_head;
  
public:
    MoveList() { m_head = NULL; }
  
    void insertNode(moveType move);
    void deleteNode(moveType move);
    void getNode(int index);
    inline moveType getHead() { return m_head->m_move; };
};
  
void MoveList::insertNode(moveType move) {
    MoveNode* newNode = new MoveNode(move);
  
    if (m_head == NULL) {
        m_head = newNode;
        return;
    }
  
    // Traverse list
    MoveNode* MoveNode = m_head;
    while (MoveNode->m_next != NULL) {
        MoveNode = MoveNode->m_next;
    }

    MoveNode->m_next = newNode;
}
