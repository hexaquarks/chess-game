#pragma once
#include "../Utilities/Move.hpp"

#include <utility>
#include <vector>
#include <tuple>
#include <iostream>

enum class Team { WHITE, BLACK };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN };

typedef std::tuple<coor2d, MoveType> moveType;
typedef std::vector<moveType> moveTypes;

class Board;

// Represents a piece
class Piece
{
public:
    Piece(Team, int, int, PieceType, const std::string&); // Constructor

    // C.67: A polymorphic class should suppress copying
    Piece(const Piece&) = delete;
    Piece(Piece&&) = delete;
    Piece& operator=(const Piece&) = delete;
    Piece& operator=(Piece&&) = delete;

    // Rule of five
    virtual ~Piece() {} 

    /* Getters */
    Team getTeam() const { return m_team; };
    PieceType getType() const { return m_type; }
    const std::string& getFileName() const { return m_filename; }
    MoveType getLastMove() const { return m_lastMove; }
    static std::shared_ptr<Piece> getLastMovedPiece() { return m_lastPiece; }
    int getX() const { return m_xPos; }
    int getY() const { return m_yPos; }
    bool hasMoved() const { return m_moved; }
    void setAsFirstMovement() { m_moved = false; }
    void addHorizontalAndVerticalMovements(Board&, std::vector<Move>&) const;
    void addDiagonalMovements(Board&, std::vector<Move>&) const;

    /* Setters */
    void setLastMove(MoveType newMove) { m_lastMove = newMove; }
    static void setLastMovedPiece(const std::shared_ptr<Piece>& piece) { m_lastPiece = piece; }

    /* Utility functions */
    virtual std::vector<Move> calcPossibleMoves(Board&) const = 0; // Pure virtual function
    void move(int x, int y, bool record = true)
    {
        if (record && (m_xPos != x || m_yPos != y)) m_moved = true;
        m_xPos = x;
        m_yPos = y;
    }

private:
    /* Static members */
    inline const static std::string fileExt = ".png"; // Pieces file extension
    inline static std::shared_ptr<Piece> m_lastPiece; // Last moved piece

    /* Class members */
    std::string m_filename; // Filename for this piece
    Team m_team; // Team this piece plays for
    PieceType m_type; // Type of this piece
    MoveType m_lastMove; // Move type of this piece
    int m_xPos; int m_yPos; // X and Y positions
    bool m_moved = false; // Whether piece has moved or not

    /* Helper function */
    std::string getColorCode() { return m_team == Team::BLACK? "b": "w"; }
};
