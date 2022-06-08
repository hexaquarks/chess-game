#pragma once
#include "../Pieces/Piece.hpp"

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
    vector<shared_ptr<Piece>> getWhitePieces() { return m_whitePieces; }
    vector<shared_ptr<Piece>> getBlackPieces() { return m_blackPieces; }
    shared_ptr<Piece> getBoardTile(int x, int y) const { return m_board[y][x]; }
    shared_ptr<King> getKing() const;
    Team getTurn() const { return m_turn; }
    void setBoardTile(int, int, shared_ptr<Piece>&, bool = true);
    void resetBoardTile(int, int, bool = true);

    // Utility functions
    vector<Move> possibleMovesFor(shared_ptr<Piece>&);
    vector<Move> calculateAllMoves();
    void switchTurn();
    bool kingIsChecked();
    void addPiece(shared_ptr<Piece>&);

private:
    // Member variables
    shared_ptr<Piece> m_board[8][8];
    Team m_turn; // White or black player's turn
    vector<shared_ptr<Piece>> m_whitePieces;
    vector<shared_ptr<Piece>> m_blackPieces;
    shared_ptr<King> m_whiteKing;
    shared_ptr<King> m_blackKing;
    bool m_isFlipped = false;

    // Private functions
    void removeIllegalMoves(vector<Move>&, shared_ptr<Piece>&);

};
