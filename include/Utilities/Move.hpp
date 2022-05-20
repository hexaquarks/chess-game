#pragma once
#include <iostream>
#include <tuple>
#include <memory>

#include "Arrow.hpp"

using namespace std;

// Forward declarations
class Piece;
class Board;

typedef pair<int, int> coor2d;
enum class MoveType { NORMAL, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, ENPASSANT, NEWPIECE, CAPTURE, INIT_SPECIAL };

class Move
{
    shared_ptr<Piece> m_selectedPiece; // Piece that is being selected
    shared_ptr<Piece> m_capturedPiece; // Captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; // Move type
    coor2d m_target; // Destination square of the piece that is being moved
    coor2d m_init; // Initial square of the piece moved
    coor2d m_special; // En passant information
    vector<Arrow> m_arrows; // List of arrows drawn at that move
    bool kingChecked = false;
    bool kingMated = false;

    static string coorRepr(const coor2d&);
    static coor2d fromRepr(char, char);

    public:
    Move(coor2d, coor2d, shared_ptr<Piece>&, MoveType, shared_ptr<Piece>&);
    Move(coor2d, coor2d, shared_ptr<Piece>&, MoveType);
    Move(Move&, shared_ptr<Piece>&, coor2d capturedPawn = make_pair(-1, -1)); // Constructor for CAPTURE, EN PASSANT
    Move(const char[]);
    vector<Arrow> getMoveArrows() { return m_arrows; }
    MoveType getMoveType() const { return m_MoveType; }
    shared_ptr<Piece> getSelectedPiece() { return m_selectedPiece; }
    shared_ptr<Piece> getCapturedPiece() { return m_capturedPiece; }
    coor2d getTarget() const { return m_target; }
    coor2d getInit() const { return m_init; }
    coor2d getSpecial() const { return m_special; }
    bool kingIsChecked() const { return kingChecked; }
    bool kingIsCheckmated() const { return kingMated; }
    void setChecked() { kingChecked = true; }
    void setCheckmate() { kingMated = true; }
    void setTarget(coor2d& target) { m_target = target; }
    void setMoveType(MoveType moveType) { m_MoveType = moveType; }
    void setCapturedPiece(shared_ptr<Piece>& capturedPiece) { m_capturedPiece = capturedPiece; }
    void setSelectedPiece(shared_ptr<Piece>& p) { m_selectedPiece = p; }
    void setMoveArrows(vector<Arrow> arrows) { m_arrows = arrows; }
    bool operator ==(Move& other)
    {
        return m_selectedPiece == other.m_selectedPiece &&
        m_MoveType == other.m_MoveType &&
        m_target == other.m_target &&
        m_init == other.m_init &&
        m_special == other.m_special;
    }
    string toString() const;
};
