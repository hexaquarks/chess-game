#pragma once
#include "Piece.hpp"
#include <vector>


// Represents a pawn
struct Pawn: public Piece
{
    public:
    Pawn(Team, int, int); // Constructor

    vector<Move> calcPossibleMoves(shared_ptr<Piece>[8][8]) const override;
    void generateCaptureMoves(vector<Move>&, shared_ptr<Piece>[8][8], int) const;
    void generateForwardMoves(vector<Move>&, shared_ptr<Piece>[8][8], int) const;
    void generateEnPassantMoves(vector<Move>&, shared_ptr<Piece>[8][8]) const;
};
