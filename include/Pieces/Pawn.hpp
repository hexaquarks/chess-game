#pragma once
#include "Piece.hpp"
#include <vector>


// Represents a pawn
struct Pawn: public Piece
{
    public:
    Pawn(Team, int, int); // Constructor

    vector<Move> calcPossibleMoves(Board&) const override;
    void generateCaptureMoves(vector<Move>&, Board&, int) const;
    void generateForwardMoves(vector<Move>&, Board&, int) const;
    void generateEnPassantMoves(vector<Move>&, Board&, int) const;
};
