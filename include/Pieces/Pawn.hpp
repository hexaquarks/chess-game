#pragma once
#include "Piece.hpp"

// Represents a pawn
struct Pawn: public Piece
{
public:
    explicit Pawn(Team, int, int); // index-based
    explicit Pawn(Team, coor2dChar&); // real coordinates

    std::vector<Move> calcPossibleMoves(Board&) const override;
    void generateCaptureMoves(std::vector<Move>&, Board&, int) const;
    void generateForwardMoves(std::vector<Move>&, Board&, int) const;
    void generateEnPassantMoves(std::vector<Move>&, Board&, int) const;
};
