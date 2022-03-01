#include "../include/Move.hpp"

// For NORMAL, INIT SPECIAL and CASTLE
Move::Move(coor2d target, coor2d initial, Piece* selectedPiece, MoveType moveType, Piece* secondPiece):
    m_target(target),m_init(initial),
    m_selectedPiece(selectedPiece), m_capturedPiece(secondPiece),
    m_MoveType(moveType) {}

// For CAPTURE and ENPASSANT
Move::Move(Move& move, Piece* secondPiece):
    m_target(move.getTarget()), m_init(move.getInit()),
    m_selectedPiece(move.getSelectedPiece()), m_capturedPiece(secondPiece),
    m_MoveType(move.getMoveType()) {}
