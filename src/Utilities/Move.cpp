#include "../../include/Utilities/Move.hpp"
#include "../../include/Components/Board.hpp"

// For NORMAL, INIT SPECIAL and CASTLE
Move::Move(
    coor2d target_, coor2d initial_, shared_ptr<Piece>& pSelectedPiece_,
    MoveType moveType_, shared_ptr<Piece>& pSecondPiece_
):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_), m_capturedPiece(pSecondPiece_),
    m_MoveType(moveType_)
{
}

Move::Move(coor2d target_, coor2d initial_, shared_ptr<Piece>& pSelectedPiece_, MoveType moveType_):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_),
    m_MoveType(moveType_)
{
}

// For CAPTURE and ENPASSANT
Move::Move(Move& move_, shared_ptr<Piece>& pSecondPiece_, coor2d capturedPawn_):
    m_target(move_.getTarget()), m_init(move_.getInit()), m_special(capturedPawn_),
    m_selectedPiece(move_.getSelectedPiece()), m_capturedPiece(pSecondPiece_),
    m_MoveType(move_.getMoveType()), kingChecked(move_.kingChecked), kingMated(move_.kingMated)
{
}
