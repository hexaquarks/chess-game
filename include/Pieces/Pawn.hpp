#pragma once
#include "Piece.hpp"
#include <vector>


// Represents a pawn
struct Pawn: public Piece {
    bool m_isFlipped = false; // Is the board direction flipped

    public:
    Pawn(Team, int, int); // Constructor

    vector<Move> calcPossibleMoves(Piece*[8][8]) const override;
    void setIsFlipped(bool isFlipped) { m_isFlipped = isFlipped; }
    bool getIsFlipped() const { return m_isFlipped; }
    void generateCaptureMoves(vector<Move>&, Piece*[8][8], int) const;
    void generateForwardMoves(vector<Move>&, Piece*[8][8], int) const;
    void generateEnPassantMoves(vector<Move>&, Piece*[8][8]) const;
    inline bool getPawnDirection(bool a, bool b) const { return (a || b) && !(a && b); }
};
