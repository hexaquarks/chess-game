#pragma once
#include "Piece.hpp"
#include <vector>


// Represents a pawn
struct Pawn: public Piece {
    Pawn(Team, int, int); // Constructor

    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
    void generateCaptureMoves(moveTypes&, Piece*[8][8], int) const;
    void generateForwardMoves(moveTypes&, Piece*[8][8], int) const;
    void generateEnPassantMoves(moveTypes&, Piece*[8][8]) const;
};
