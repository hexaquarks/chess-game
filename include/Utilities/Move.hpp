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
public:
    Move(const coor2d&&, const coor2d&, const shared_ptr<Piece>&, MoveType, const shared_ptr<Piece>&);
    Move(const coor2d&&, const coor2d&, const shared_ptr<Piece>&, MoveType);
    Move(const Move&, const shared_ptr<Piece>&, const coor2d& = make_pair(-1, -1)); // Constructor for CAPTURE, EN PASSANT
    Move(const string&);

    vector<Arrow> getMoveArrows() { return m_arrows; }
    MoveType getMoveType() const { return m_MoveType; }
    shared_ptr<Piece> getSelectedPiece() const { return m_selectedPiece; }
    shared_ptr<Piece> getCapturedPiece() const { return m_capturedPiece; }
    coor2d getTarget() const { return m_target; }
    coor2d getInit() const { return m_init; }
    coor2d getSpecial() const { return m_special; }

    bool kingIsChecked() const { return m_kingChecked; }
    bool hasNoMovesAvailable() const { return m_noMovesAvailable; }
    bool kingIsCheckmated() const { return m_kingChecked && m_noMovesAvailable; }

    void setChecked(bool checked_ = true) { m_kingChecked = checked_; }
    void setNoMovesAvailable(bool noMoves_ = true) { m_noMovesAvailable = noMoves_; }
    void setTarget(coor2d& target_) { m_target = target_; }
    void setMoveType(MoveType moveType_) { m_MoveType = moveType_; }
    void setCapturedPiece(shared_ptr<Piece>& capturedPiece_) { m_capturedPiece = capturedPiece_; }
    void setSelectedPiece(shared_ptr<Piece>& piece_) { m_selectedPiece = piece_; }
    void setMoveArrows(vector<Arrow> arrows_) { m_arrows = arrows_; }

    bool operator ==(const Move&);
    string toString() const;

private:
    shared_ptr<Piece> m_selectedPiece; // Piece that is being selected
    shared_ptr<Piece> m_capturedPiece; // Captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType; // Move type
    coor2d m_target; // Destination square of the piece that is being moved
    coor2d m_init; // Initial square of the piece moved
    coor2d m_special; // En passant information
    vector<Arrow> m_arrows; // List of arrows drawn at that move
    bool m_kingChecked = false; 
    bool m_noMovesAvailable = false; // Flag that captures the availablility of possible moves in the position

    static string coorRepr(const coor2d&);
    static coor2d fromRepr(char, char);
};
