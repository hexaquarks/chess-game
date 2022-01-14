#pragma once
#include "Piece.hpp"
#include <vector>

// Represents a pawn
class Pawn : public Piece {
    inline static Pawn* lastPawn = nullptr;

    public:
    Pawn(Team, int, int); // Constructor
    static void setLastPawn(Pawn *p) { lastPawn = p; }
    static Pawn* getLastPawn() { return lastPawn; }

    moveTypes calcPossibleMoves(Piece*[8][8]) const override;
    void generateCaptureMoves(moveTypes&, Piece*[8][8], int) const;
    void generateForwardMoves(moveTypes&, Piece*[8][8], int) const;
    void generateEnPassantMoves(moveTypes&, Piece*[8][8]) const;
};
