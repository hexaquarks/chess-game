#pragma once
#include "Piece.hpp"

// Represents a pawn
struct Pawn: public Piece
{
    public:
    Pawn(Team, int, int); // Constructor

    std::vector<Move> calcPossibleMoves(Board&) const override;
    void generateCaptureMoves(std::vector<Move>&, Board&, int) const;
    void generateForwardMoves(std::vector<Move>&, Board&, int) const;
    void generateEnPassantMoves(std::vector<Move>&, Board&, int) const;
};
