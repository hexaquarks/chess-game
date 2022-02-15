#pragma once
#include "Piece.hpp"

typedef pair<int, int> coor2d;

class Move {
    Piece* m_selectedPiece; // Piece that is being selected
    Piece* m_capturedPiece; // Captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; // Move type
    coor2d m_target; // Destination square of the piece that is being moved
    coor2d m_init; // Initial square of the piece moved

    public:
    Move(coor2d&, coor2d&, Piece*, MoveType); // Constructor for NORMAL, INIT SPECIAL and CASTLE
    Move(Move&, Piece*); // Constructor for CAPTURE, EN PASSANT
    MoveType getMoveType() const { return m_MoveType; };
    Piece* getSelectedPiece() { return m_selectedPiece; };
    Piece* getCapturedPiece() { return m_capturedPiece; };
    coor2d getTarget() const { return m_target; }
    coor2d getInit() const { return m_init; }
    void setTarget(coor2d& target) { m_target = target; }
    void setMoveType(MoveType moveType) { m_MoveType = moveType; }
    void setCapturedPiece(Piece* capturedPiece) { m_capturedPiece = capturedPiece; }

};
