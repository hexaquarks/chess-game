#pragma once
#include <list>
#include "../Pieces/Piece.hpp"
#include "../Pieces/Pawn.hpp"
#include "../Pieces/Rook.hpp"
#include "../Pieces/Knight.hpp"
#include "../Pieces/Bishop.hpp"
#include "../Pieces/King.hpp"
#include "../Pieces/Queen.hpp"


class Board 
{
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

    public:
    Board(): m_turn(Team::WHITE) { reset(); } // Constructor
    void reset(); // Resets the board

    // Getters and setters 
    shared_ptr<Piece> getBoardTile(int x, int y) const { return m_board[y][x]; }
    King* getKing() const { return (m_turn == Team::WHITE)? m_whiteKing.get(): m_blackKing.get(); }
    Team getTurn() const { return m_turn; }
    void setBoardTile(int, int, shared_ptr<Piece>&, bool record = true);
    void resetBoardTile(int, int, bool record_ = true);
    void switchTurn() { m_turn = (m_turn == Team::WHITE)? Team::BLACK: Team::WHITE; }

    // Utility functions
    vector<Move> possibleMovesFor(shared_ptr<Piece>& piece) { return piece->calcPossibleMoves(m_board); }
    vector<Move> calculateAllMoves();
    bool kingIsChecked() { return getKing()->isChecked(m_board); }
    void addPiece(shared_ptr<Piece>&);
};
