#pragma once
#include "Piece.hpp"
#include <vector>


// Represents a pawn
struct Pawn: public Piece 
{
    public:
    Pawn(Team, int, int); // Constructor

    vector<Move> calcPossibleMoves(Piece*[8][8]) const override;
    void generateCaptureMoves(vector<Move>&, Piece*[8][8], int) const;
    void generateForwardMoves(vector<Move>&, Piece*[8][8], int) const;
    void generateEnPassantMoves(vector<Move>&, Piece*[8][8]) const;
};
