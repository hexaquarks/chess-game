#pragma once
#include "Piece.hpp"
#include "../Board.hpp"

// Represents a king
class King: public Piece
{
public:
    explicit King(Team, int, int); // index-based
    explicit King(Team, coor2dChar&); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
    bool isChecked(Board&) const;

    static void swapPieces(Board& board_, int rank1_, int file1_, int rank2_, int file2_)
    {
        auto& first = board_.getBoardTile(file1_, rank1_);
        auto& second = board_.getBoardTile(file2_, rank2_);
        board_.setBoardTile(file1_, rank1_, second, false);
        board_.setBoardTile(file2_, rank2_, first, false);
    }

private:
    std::vector<Move> possibleMovesNoCheck(Board&) const;
    bool canCastleKingSide(Board&) const;
    bool canCastleQueenSide(Board&) const;
};
