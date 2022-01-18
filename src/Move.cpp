#include "../include/Move.hpp"


// for NORMAL, INIT SPECIAL and CASTLE
Move::Move(int x, int y, Piece* selectedPiece, MoveType moveType)
    : m_xTarget(x),
      m_yTarget(y),
      m_selectedPiece(selectedPiece),
      m_MoveType(moveType) {};

// for CAPTURE and ENPASSANT
Move::Move(int x, int y, Piece* selectedPiece, Piece* secondPiece, MoveType moveType)
    : m_xTarget(x),
      m_yTarget(y),
      m_selectedPiece(selectedPiece),
      m_capturedPiece(secondPiece),
      m_MoveType(moveType) {};


moveFullCoords Move::getMoveCoords() const {
     return make_tuple(
         make_pair( m_selectedPiece->getX(), m_selectedPiece->getY()),
         make_pair( m_xTarget, m_yTarget)
     );
};