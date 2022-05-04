#include "../../include/Utilities/Move.hpp"

// For NORMAL, INIT SPECIAL and CASTLE
Move::Move(coor2d target_, coor2d initial_, Piece* pSelectedPiece_, MoveType moveType_, Piece* pSecondPiece_):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_), m_capturedPiece(pSecondPiece_),
    m_MoveType(moveType_) {}

// For CAPTURE and ENPASSANT
Move::Move(Move& move_, Piece* pSecondPiece_, coor2d capturedPawn_):
    m_target(move_.getTarget()), m_init(move_.getInit()), m_special(capturedPawn_),
    m_selectedPiece(move_.getSelectedPiece()), m_capturedPiece(pSecondPiece_),
    m_MoveType(move_.getMoveType()) {}
