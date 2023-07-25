#pragma once
#include "../Utilities/Move.hpp"

#include <utility>
#include <vector>
#include <tuple>
#include <iostream>

enum class Team { WHITE, BLACK };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN };

std::ostream& operator<<(std::ostream&, const PieceType&);
std::ostream& operator<<(std::ostream&, Team);

typedef std::tuple<coor2d, MoveType> moveType;
typedef std::vector<moveType> moveTypes;
typedef std::pair<int, char> coor2dChar;

class Board;

// Represents a piece
class Piece
{
public:
    explicit Piece(Team, int, int, PieceType, const std::string&);
    explicit Piece(Team, coor2dChar&, PieceType, const std::string&); 

    // C.67: A polymorphic class should suppress copying
    Piece(const Piece&) = delete;
    Piece(Piece&&) = delete;
    Piece& operator=(const Piece&) = delete;
    Piece& operator=(Piece&&) = delete;

    // Rule of five
    virtual ~Piece() = default; 

    /* Getters */
    Team getTeam() const { return m_team; };
    PieceType getType() const { return m_type; }
    const std::string& getFileName() const { return m_filename; }
    MoveType getLastMove() const { return m_lastMove; }
    static std::shared_ptr<Piece> getLastMovedPiece() { return m_lastPiece; }
    int getRank() const { return m_rank; }
    int getFile() const { return m_file; }
    bool hasMoved() const { return m_moved; }
    void setAsFirstMovement() { m_moved = false; }
    void addHorizontalAndVerticalMovements(Board&, std::vector<Move>&) const;
    void addDiagonalMovements(Board&, std::vector<Move>&) const;

    /* Setters */
    void setLastMove(MoveType newMove) { m_lastMove = newMove; }
    static void setLastMovedPiece(const std::shared_ptr<Piece>& piece) { m_lastPiece = piece; }

    /* Utility functions */
    virtual std::vector<Move> calcPossibleMoves(Board&) const = 0; // Pure virtual function
    void move(int rank_, int file_, bool record = true)
    {
        if (record && (m_rank != rank_ || m_file != file_)) m_moved = true;
        m_rank = rank_;
        m_file = file_;
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
    int m_rank; 
    int m_file;  
    bool m_moved = false; // Whether piece has moved or not

    /* Helper function */
    std::string getColorCode() { return m_team == Team::BLACK? "b": "w"; }
};
