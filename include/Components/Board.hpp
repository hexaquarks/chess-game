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
    Piece* m_board[8][8];
    Team m_turn; // White or black player's turn
    vector<Piece*> m_whitePieces;
    vector<Piece*> m_blackPieces;
    King* m_whiteKing;
    King* m_blackKing;
    bool m_isFlipped = false;

    // Private functions
    void freeMemory();
    void removeIllegalMoves(vector<Move>&, Piece*);

    public:
    Board(): m_turn(Team::WHITE) { reset(); } // Constructor
    ~Board() { freeMemory(); } // Destructor

    void reset(); // Resets the board

    // Getters and setters 
    Piece* getBoardTile(int x, int y) const { return m_board[y][x]; }
    King* getKing() const { return (m_turn == Team::WHITE)? m_whiteKing: m_blackKing; }
    Team getTurn() const { return m_turn; }
    void setBoardTile(int, int, Piece*, bool record = true);
    void switchTurn() { m_turn = (m_turn == Team::WHITE)? Team::BLACK: Team::WHITE; }

    // Utility functions
    vector<Move> possibleMovesFor(Piece* piece) { return piece->calcPossibleMoves(m_board); }
    vector<Move> calculateAllMoves();
    bool kingIsChecked() { return getKing()->isChecked(m_board); }
    void addPiece(Piece*);
};
