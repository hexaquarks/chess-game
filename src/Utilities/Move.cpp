#include "../../include/Utilities/Move.hpp"
#include "../../include/Components/Board.hpp"

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

bool Move::operator ==(const Move& other_)
{
    return m_selectedPiece == other_.m_selectedPiece &&
    m_MoveType == other_.m_MoveType &&
    m_target == other_.m_target &&
    m_init == other_.m_init &&
    m_special == other_.m_special;
}

coor2d Move::fromRepr(char first, char second) {
    return {first - 'a', 8 - (second - '0')};
}

string Move::coorRepr(const coor2d& pos) {
    char move[] {static_cast<char>('a' + pos.first), static_cast<char>('8' - pos.second), '\0'};
    return string(move);
}

string Move::toString() const {
    return coorRepr(m_init) + coorRepr(m_target);
}
