#pragma once
#include "Piece.hpp"
#include "../../include/Components/Board.hpp"

// Represents a king
class King: public Piece
{
public:
    King(Team, int, int); // Constructor
    std::vector<Move> calcPossibleMoves(Board&) const override;
    bool isChecked(Board&) const;

    static void swapPieces(Board& board_, int x, int y, int X, int Y)
    {
        std::shared_ptr<Piece> first = board_.getBoardTile(y, x);
        std::shared_ptr<Piece> second = board_.getBoardTile(Y, X);
        board_.setBoardTile(y, x, second, false);
        board_.setBoardTile(Y, X, first, false);
    }

private:
    std::vector<Move> possibleMovesNoCheck(Board&) const;
    bool canCastleKingSide(Board&) const;
    bool canCastleQueenSide(Board&) const;
};
