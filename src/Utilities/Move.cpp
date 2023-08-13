#include "../../include/Utilities/Move.hpp"
#include "../../include/Components/Board.hpp"

namespace // anonymous namespace
{
    coor2d fromRepr(char first_, char second_) 
    {
        return {first_ - 'a', 8 - (second_ - '0')};
    }

    std::string coorRepr(const coor2d& pos_) 
    {
        char move[] {static_cast<char>('a' + pos_.first), static_cast<char>('8' - pos_.second), '\0'};
        return std::string(move);
    }

    std::string toString(const coor2d& init_, const coor2d& target_)  
    {
        return coorRepr(init_) + coorRepr(target_);
    }
}
// For NORMAL, INIT SPECIAL and CASTLE
Move::Move(
    const coor2d&& target_, const coor2d& initial_, const std::shared_ptr<Piece>& pSelectedPiece_,
    MoveType moveType_, const std::shared_ptr<Piece>& pSecondPiece_
):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_), m_capturedPiece(pSecondPiece_),
    m_MoveType(moveType_)
{
}

Move::Move(
    const coor2d&& target_, const coor2d& initial_, const std::shared_ptr<Piece>& pSelectedPiece_, MoveType moveType_
):
    m_target(target_), m_init(initial_),
    m_selectedPiece(pSelectedPiece_),
    m_MoveType(moveType_)
{
}

// For CAPTURE and ENPASSANT
Move::Move(
    const Move& move_, const std::shared_ptr<Piece>& pSecondPiece_, const coor2d& capturedPawn_
):
    m_target(move_.getTarget()), 
    m_init(move_.getInit()), 
    m_special(capturedPawn_),
    m_selectedPiece(move_.getSelectedPiece()), 
    m_capturedPiece(pSecondPiece_),
    m_MoveType(move_.getMoveType()), 
    m_kingChecked(move_.m_kingChecked),
    m_noMovesAvailable(move_.m_noMovesAvailable)
{
}

// Move from string representation
Move::Move(const std::string& repr)
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

bool Move::operator!=(const Move& other_)
{
     return !(*this == other_);
}

// free functions

std::pair<char, int> findLetterCoord(const coor2d& target_) 
{
    char letter = 'a' + target_.first;
    return {letter, 8 - target_.second};
}

std::string parseMoveHelper(const Move& move_, int moveNumber_, bool showNumber_, bool showDots_) 
{
    std::string text = (showNumber_)
        ? std::to_string(moveNumber_) + "."
        : (showDots_)? std::to_string(moveNumber_-1) + "..." : "";
    MoveType moveType = move_.getMoveType();

    // Side cases
    if (moveType == MoveType::CASTLE_KINGSIDE) { return text + "O-O"; }
    if (moveType == MoveType::CASTLE_QUEENSIDE) { return text + "O-O-O"; }

    std::pair<char, int> letterCoord = findLetterCoord(move_.getTarget());
    std::string letterCoordString = static_cast<char>(letterCoord.first) + std::to_string(letterCoord.second);

    switch (move_.getSelectedPiece()->getType())
    {
        case PieceType::PAWN:
            // TODO fix promotion
            if (moveType == MoveType::CAPTURE || moveType == MoveType::ENPASSANT)
            {
                return text + (static_cast<char>('a' + move_.getInit().second)) + "x" + letterCoordString;
            } break;
        case PieceType::KNIGHT: text += "N"; break;
        case PieceType::BISHOP: text += "B"; break;
        case PieceType::ROOK: text += "R"; break;
        case PieceType::QUEEN: text += "Q"; break;
        case PieceType::KING: text += "K"; break;
    }
    return text + std::string((moveType == MoveType::CAPTURE)? "x" : "") + letterCoordString;
}

std::string parseMove(const Move& move_, int moveNumber_, bool showNumber_, bool showDots_)
{
    std::string text = parseMoveHelper(move_, moveNumber_, showNumber_, showDots_);
    if (move_.kingIsCheckmated()) return text + std::string("#");
    if (move_.kingIsChecked()) return text + std::string("+");
    return text;
}

std::ostream& operator<<(std::ostream& os_, const Move& move_)
{
    return os_;
}