#include "../include/Move.hpp"


// for NORMAL and INIT SPECIAL 
Move::Move(int x, int y, Piece* selectedPiece, moveType* moveType)
    : m_xTarget(x),
      m_yTarget(y),
      m_selectedPiece(selectedPiece),
      m_moveType(moveType) {};

// for CAPTURE, ENPASSANT, CASTLE
Move::Move(int x, int y, Piece* selectedPiece, Piece* secondPiece, moveType* moveType, moveFullCoords secondPieceCoords)
    : m_xTarget(x),
      m_yTarget(y),
      m_selectedPiece(selectedPiece),
      m_takenOrMovedPiece(secondPiece),
      m_moveType(moveType),
      m_secondPieceCoords(secondPieceCoords) {};

moveFullCoords Move::getMoveCoords() const {
     return make_tuple(
         make_pair( m_selectedPiece->getX(), m_selectedPiece->getY()),
         make_pair( m_xTarget, m_yTarget)
     );
};