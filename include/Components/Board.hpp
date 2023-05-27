#pragma once
#include "../Pieces/Piece.hpp"
#include "../Utilities/Move.hpp"

#include <list>
#include "memory"

using namespace std;

class King;
class Move;
enum class Team;

class Board
{
public:
    Board(); // Constructor
    void reset(); // Resets the board

    // Getters and setters
    vector<shared_ptr<Piece>> getWhitePieces() const { return m_whitePieces; }
    vector<shared_ptr<Piece>> getBlackPieces() const { return m_blackPieces; }
    shared_ptr<Piece> getBoardTile(int x, int y) const { return m_board[y][x]; }
    shared_ptr<King> getKing() const;
    Team getTurn() const { return m_turn; }
    void setBoardTile(int, int, shared_ptr<Piece>&, bool = true);
    void resetBoardTile(int, int, bool = true);

    // Utility functions
    vector<Move> possibleMovesFor(const shared_ptr<Piece>&);
    const vector<Move>& getAllCurrentlyAvailableMoves() const { return m_allCurrentlyAvailableMoves; }
    void updateAllCurrentlyAvailableMoves();
    void switchTurn();
    bool kingIsChecked();
    bool isFlipped() { return m_isFlipped; }
    void addPiece(const shared_ptr<Piece>&);
    void flipBoard() { m_isFlipped = !m_isFlipped; }

private:
    // Member variables
    shared_ptr<Piece> m_board[8][8];
    Team m_turn; // White or black player's turn
    vector<shared_ptr<Piece>> m_whitePieces;
    vector<shared_ptr<Piece>> m_blackPieces;
    shared_ptr<King> m_whiteKing;
    shared_ptr<King> m_blackKing;
    bool m_isFlipped = false;
    vector<Move> m_allCurrentlyAvailableMoves{};

    // Private functions
    void removeIllegalMoves(vector<Move>&, shared_ptr<Piece>&);
};
