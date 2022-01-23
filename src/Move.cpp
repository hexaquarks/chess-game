#include "../include/Move.hpp"


// For NORMAL, INIT SPECIAL and CASTLE
Move::Move(int x, int y, int xs, int ys, Piece* selectedPiece, MoveType moveType):
  m_xTarget(x),
  m_yTarget(y),
  m_xInit(xs),
  m_yInit(ys),
  m_selectedPiece(selectedPiece),
  m_capturedPiece(nullptr),
  m_MoveType(moveType) {}

// For CAPTURE and ENPASSANT
Move::Move(int x, int y,int xs, int ys, 
  Piece* selectedPiece, Piece* secondPiece, MoveType moveType):
  
  m_xTarget(x),
  m_yTarget(y),
  m_xInit(xs),
  m_yInit(ys),
  m_selectedPiece(selectedPiece),
  m_capturedPiece(secondPiece),
  m_MoveType(moveType) {}

moveFullCoords Move::getMoveCoords() const {
  return make_tuple(
    make_pair(m_selectedPiece->getX(), m_selectedPiece->getY()),
    make_pair(m_xTarget, m_yTarget)
  );
}
