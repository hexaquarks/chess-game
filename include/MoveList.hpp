#include "Piece.hpp"
#include <iostream>
  
class MoveNode {
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
  
    void insertNode(moveType move) const;
    void deleteNode(moveType move) const;
    inline moveType getHead() { return m_head->m_move; };
};