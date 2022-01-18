#include "../include/Move.hpp"


Move::Move(int x, int y, Piece* selectedPiece, Piece* secondPiece, moveType moveType, Move* lastMove)
    : m_xTarget(x),
      m_yTarget(y),
      m_selectedPiece(secondPiece),
      m_takenOrMovedPiece(secondPiece),
      m_moveType(moveType),
      m_lastMove(lastMove) {};