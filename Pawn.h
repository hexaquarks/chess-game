#pragma once
#include "Piece.h"
#include <vector>

// Represents a pawn
struct Pawn : public Piece {
    Pawn(Team, int, int); // Constructor
    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
    void generateCaptureMoves(Pawn p, moveTypes moves, Piece*[8][8], int) const;
    void generateForwardMoves(Pawn p, moveTypes moves, Piece*[8][8], int) const;
    void generateEnPassantMoves(Pawn p, moveTypes moves, Piece*[8][8]) const;

};
