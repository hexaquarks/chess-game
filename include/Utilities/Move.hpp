#pragma once
#include <iostream>
#include <tuple>
#include "Arrow.hpp"
using namespace std;

typedef pair<int, int> coor2d;

class Piece;

enum class MoveType { NORMAL, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, ENPASSANT, NEWPIECE, CAPTURE, INIT_SPECIAL };

class Move {
    Piece* m_selectedPiece = nullptr; // Piece that is being selected
    Piece* m_capturedPiece = nullptr; // Captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; // Move type
    coor2d m_target; // Destination square of the piece that is being moved
    coor2d m_init; // Initial square of the piece moved
    coor2d m_special; // En passant information
    vector<Arrow> m_arrows; // List of arrows drawn at that move

    public:
    Move(coor2d, coor2d, Piece*, MoveType, Piece* secondPiece = nullptr);
    Move(Move&, Piece*, coor2d capturedPawn = make_pair(-1, -1)); // Constructor for CAPTURE, EN PASSANT
    vector<Arrow> getMoveArrows() { return m_arrows; }
    MoveType getMoveType() const { return m_MoveType; }
    Piece* getSelectedPiece() { return m_selectedPiece; }
    Piece* getCapturedPiece() { return m_capturedPiece; }
    coor2d getTarget() const { return m_target; }
    coor2d getInit() const { return m_init; }
    coor2d getSpecial() const { return m_special; }
    void setTarget(coor2d& target) { m_target = target; }
    void setMoveType(MoveType moveType) { m_MoveType = moveType; }
    void setCapturedPiece(Piece* capturedPiece) { m_capturedPiece = capturedPiece; }
    void setSelectedPiece(Piece* p) { m_selectedPiece = p; }
    void setMoveArrows(vector<Arrow> arrows) { m_arrows = arrows; }
    bool operator ==(Move& other) {
        return m_selectedPiece == other.m_selectedPiece &&
        m_MoveType == other.m_MoveType &&
        m_target == other.m_target &&
        m_init == other.m_init &&
        m_special == other.m_special;
    }
};
