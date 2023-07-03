#pragma once
#include "../Pieces/Piece.hpp"
#include "../Utilities/Move.hpp"

#include <list>
#include "memory"

class King;
class Move;
enum class Team;

class Board
{
public:
    Board(); // Constructor
    void reset(); // Resets the board

    // Getters and setters
    std::vector<std::shared_ptr<Piece>> getWhitePieces() const { return m_whitePieces; }
    std::vector<std::shared_ptr<Piece>> getBlackPieces() const { return m_blackPieces; }
    std::shared_ptr<Piece>& getBoardTile(int x, int y) { return m_board[y][x]; }
    std::shared_ptr<King> getKing() const;
    Team getTurn() const { return m_turn; }
    void setIsKingChecked(bool isKingChecked_) { m_isKingChecked = isKingChecked_; }
    void setBoardTile(int, int, std::shared_ptr<Piece>&, bool = true);
    void resetBoardTile(int, int, bool = true);

    // Utility functions
    std::vector<Move> possibleMovesFor(const std::shared_ptr<Piece>&);
    const std::vector<Move>& getAllCurrentlyAvailableMoves() const { return m_allCurrentlyAvailableMoves; }
    void updateAllCurrentlyAvailableMoves();
    void switchTurn();
    bool kingIsChecked();
    bool isFlipped() { return m_isFlipped; }
    bool isKingChecked() { return m_isKingChecked; }
    void addPiece(const std::shared_ptr<Piece>&);
    void flipBoard() { m_isFlipped = !m_isFlipped; }
    void checkIfMoveMakesKingChecked(const std::shared_ptr<Move>&);
    bool areThereNoMovesAvailableAtCurrentPosition() { return m_currentlyNoMovesAvailable; }
    void setAreThereNoMovesAvailableAtCurrentPosition(bool b_) { m_currentlyNoMovesAvailable = b_; }

private:
    // Member variables
    std::shared_ptr<Piece> m_board[8][8];
    Team m_turn; // White or black player's turn
    std::vector<std::shared_ptr<Piece>> m_whitePieces;
    std::vector<std::shared_ptr<Piece>> m_blackPieces;
    std::shared_ptr<King> m_whiteKing;
    std::shared_ptr<King> m_blackKing;
    bool m_isFlipped = false;
    std::vector<Move> m_allCurrentlyAvailableMoves;
    bool m_isKingChecked = false;
    bool m_currentlyNoMovesAvailable = false;

    // Private functions
    void removeIllegalMoves(std::vector<Move>&, std::shared_ptr<Piece>&);
};
