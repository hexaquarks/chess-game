#pragma once
#include <utility>
#include <vector>
#include <tuple>
#include <iostream>
#include "../Utilities/Move.hpp"
using namespace std;

enum class Team { WHITE, BLACK };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY };

typedef tuple<pair<int, int>, MoveType> moveType;
typedef vector<moveType> moveTypes;


// Represents a piece
class Piece {
    /* Static members */
    inline const static string fileExt = ".png"; // Pieces file extension
    inline static Piece* m_lastPiece = nullptr; // Last moved piece

    /* Class members */
    string m_filename; // Filename for this piece
    Team m_team; // Team this piece plays for
    PieceType m_type; // Type of this piece
    MoveType m_lastMove; // Move type of this piece
    int m_xPos; int m_yPos; // X and Y positions
    bool m_moved = false; // Whether piece has moved or not

    /* Helper function */
    string getColorCode() { return m_team == Team::BLACK? "b": "w"; }

    public:
    Piece(Team, int, int, PieceType, string); // Constructor
    virtual ~Piece() {} // Virtual destructor

    /* Getters */
    Team getTeam() const { return m_team; };
    PieceType getType() const { return m_type; }
    string getFileName() const { return m_filename; }
    MoveType getLastMove() const { return m_lastMove; }
    static Piece* getLastMovedPiece() { return m_lastPiece; }
    int getX() const { return m_xPos; }
    int getY() const { return m_yPos; }
    bool hasMoved() const { return m_moved; }
    vector<Move> getHorizontalAndVerticalMovements(Piece*[8][8]) const;
    vector<Move> getDiagonalMovements(Piece*[8][8]) const;

    /* Setters */
    void setLastMove(MoveType newMove) { m_lastMove = newMove; }
    static void setLastMovedPiece(Piece* piece) { m_lastPiece = piece; }

    /* Utility functions */
    virtual vector<Move> calcPossibleMoves(Piece*[8][8]) const = 0; // Pure virtual function
    void move(int x, int y, bool record = true) {
        if (record && (m_xPos != x || m_yPos != y)) m_moved = true;
        m_xPos = x;
        m_yPos = y;
    }
};
