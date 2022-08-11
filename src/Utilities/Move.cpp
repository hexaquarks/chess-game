#include "../../include/Utilities/Move.hpp"
#include "../../include/Components/Board.hpp"

namespace // anonymous namespace
{
    coor2d fromRepr(char first_, char second_) 
    {
        return {first_ - 'a', 8 - (second_ - '0')};
    }

    string coorRepr(const coor2d& pos_) 
    {
        char move[] {static_cast<char>('a' + pos_.first), static_cast<char>('8' - pos_.second), '\0'};
        return string(move);
    }

    string toString(const coor2d& init_, const coor2d& target_)  
    {
        return coorRepr(init_) + coorRepr(target_);
    }
}
// For NORMAL, INIT SPECIAL and CASTLE
Move::Move(
    const coor2d&& target_, const coor2d& initial_, const shared_ptr<Piece>& pSelectedPiece_,
    MoveType moveType_, const shared_ptr<Piece>& pSecondPiece_
):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_), m_capturedPiece(pSecondPiece_),
    m_MoveType(moveType_)
{
}

Move::Move(
    const coor2d&& target_, const coor2d& initial_, const shared_ptr<Piece>& pSelectedPiece_, MoveType moveType_
):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_),
    m_MoveType(moveType_)
{
}

// For CAPTURE and ENPASSANT
Move::Move(
    const Move& move_, const shared_ptr<Piece>& pSecondPiece_, const coor2d& capturedPawn_
):
    m_target(move_.getTarget()), m_init(move_.getInit()), m_special(capturedPawn_),
    m_selectedPiece(move_.getSelectedPiece()), m_capturedPiece(pSecondPiece_),
    m_MoveType(move_.getMoveType()), m_kingChecked(move_.m_kingChecked),
    m_noMovesAvailable(move_.m_noMovesAvailable)
{
}

// Move from string representation
Move::Move(const string& repr)
{
    if (repr.length() != 4) return; // Safety check
    if (repr == "0000") return; // Null move
    m_init = fromRepr(repr[0], repr[1]);
    m_target = fromRepr(repr[2], repr[3]);
}

bool Move::operator==(const Move& other_)
{
    return m_selectedPiece == other_.m_selectedPiece &&
    m_MoveType == other_.m_MoveType &&
    m_target == other_.m_target &&
    m_init == other_.m_init &&
    m_special == other_.m_special;
}

