#pragma once
#include "Piece.hpp"

typedef pair<int, int> coor2d;
typedef tuple<coor2d, coor2d> moveFullCoords;


class Move {
    moveFullCoords m_secondPieceCoords; // Last move 
    Piece* m_selectedPiece; // Piece that is being selected
    Piece* m_capturedPiece; // Captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; // Move type
    int m_xTarget; int m_yTarget; // Destination square of the piece that is being moved
    int m_xInit; int m_yInit; // Initial square of the piece moved

    public:
    Move(int, int, int, int, Piece*, MoveType); // Constructor for NORMAL, INIT SPECIAL and CASTLE
    Move(int, int, int, int, Piece*, Piece*, MoveType); // Constructor for CAPTURE, EN PASSANT
    MoveType getMoveType() const { return m_MoveType; };
    coor2d getOriginalSquare() const { return make_pair(m_xInit, m_yInit); };
    Piece* getSelectedPiece() const { return m_selectedPiece; };
    Piece* getCapturedPiece() const { return m_capturedPiece; };
    int getXTarget() const { return m_xTarget; }
    int getYTarget() const { return m_yTarget; }
    int getXInit() const { return m_xInit; }
    int getYInit() const { return m_yInit; }

    moveFullCoords getMoveCoords() const;
};
