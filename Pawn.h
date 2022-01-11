#pragma once
#include "Piece.h"
#include <vector>

// Represents a pawn
struct Pawn : public Piece {
    Pawn(Team, int, int); // Constructor

    Pawn* lastPawn = nullptr;
    void setLastPawn(Pawn *p) { lastPawn = p; }
    Pawn* getLastPawn() { return lastPawn; }

    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
    void generateCaptureMoves(moveTypes &moves, Piece*[8][8], int) const;
    void generateForwardMoves(moveTypes &moves, Piece*[8][8], int) const;
    void generateEnPassantMoves(Pawn p,moveTypes &moves, Piece*[8][8]) const;

};
