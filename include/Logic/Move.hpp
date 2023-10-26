#pragma once

#include <iostream>
#include <tuple>
#include <memory>

#include "../Utilities/Arrow.hpp"
#include <optional>

// Forward declarations
class Piece;
class Board;

enum class MoveType { NORMAL, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, ENPASSANT, NEWPIECE, CAPTURE, INIT_SPECIAL };

typedef std::pair<int, char> coor2dChar;

class Move
{
public:
    Move(const coor2d&&, const coor2d&, const std::shared_ptr<Piece>&, MoveType, const std::shared_ptr<Piece>&);
    Move(const coor2d&&, const coor2d&, const std::shared_ptr<Piece>&, MoveType);
    //Move(const coor2dChar&&, const coor2dChar&, const std::shared_ptr<Piece>&, MoveType);
    Move(const Move&, const std::shared_ptr<Piece>&, const coor2d& = std::make_pair(-1, -1)); // Constructor for CAPTURE, EN PASSANT
    Move(const std::string&);
    Move() = default;

    const std::vector<Arrow>& getMoveArrows() { return m_arrows; }
    MoveType getMoveType() const { return m_MoveType; }
    const std::shared_ptr<Piece>& getSelectedPiece() const { return m_selectedPiece; }
    std::shared_ptr<Piece>& getSelectedPiece() { return m_selectedPiece; }

    const std::shared_ptr<Piece>& getCapturedPiece() const { return m_capturedPiece; }
    std::shared_ptr<Piece>& getCapturedPiece() { return m_capturedPiece; }

    const coor2d& getTarget() const { return m_target; }
    const coor2d& getInit() const { return m_init; }
    const std::optional<coor2d>& getEnPassantCapturedPieceInitialPos() const { return m_enPassantInitialPos; }

    bool kingIsChecked() const { return m_kingChecked; }
    bool hasNoMovesAvailable() const { return m_noMovesAvailable; }
    bool kingIsCheckmated() const { return m_kingChecked && m_noMovesAvailable; }

    void setChecked(bool checked_ = true) { m_kingChecked = checked_; }
    void setNoMovesAvailable(bool noMoves_ = true) { m_noMovesAvailable = noMoves_; }
    void setTarget(const coor2d& target_) { m_target = target_; }
    void setMoveType(MoveType moveType_) { m_MoveType = moveType_; }
    void setCapturedPiece(const std::shared_ptr<Piece>& capturedPiece_) { m_capturedPiece = capturedPiece_; }
    void setSelectedPiece(const std::shared_ptr<Piece>& piece_) { m_selectedPiece = piece_; }
    void setMoveArrows(const std::vector<Arrow> arrows_) { m_arrows = arrows_; }

    bool operator==(const Move&);
    bool operator!=(const Move&);

private:
    std::shared_ptr<Piece> m_selectedPiece; // Piece that is being selected
    std::shared_ptr<Piece> m_capturedPiece; // Captured piece, the moved rook in castling, or taken pawn in en passant
    MoveType m_MoveType = MoveType::CAPTURE; // Move type
    coor2d m_target = {0, 0}; // Destination square of the piece that is being moved
    coor2d m_init = {0, 0}; // Initial square of the piece moved
    std::optional<coor2d> m_enPassantInitialPos; // En passant information
    std::vector<Arrow> m_arrows; // List of arrows drawn at that move
    bool m_kingChecked = false; 
    bool m_noMovesAvailable = false; // Flag that captures the availablility of possible moves in the position
};

std::pair<char, int> findLetterCoord(const coor2d& target_);
std::string parseMoveHelper(const Move& move_, int moveNumber_, bool showNumber_, bool showDots_);
std::string parseMove(const Move& move_, int moveNumber_, bool showNumber_, bool showDots_ = false);

std::ostream& operator<<(std::ostream&, const Move&);