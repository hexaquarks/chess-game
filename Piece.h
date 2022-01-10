#pragma once
#include <utility>
#include <vector>
#include <tuple>
#include <iostream>
using namespace std;


enum class Team { WHITE, BLACK };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY };
enum class MoveType { NORMAL, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, ENPASSANT, NEWPIECE, CAPTURE, INIT_SPECIAL };


typedef tuple<pair<int, int>, MoveType> moveType;
typedef vector<moveType> moveTypes;


class Piece {
    string filename; // the filename for this piece
    Team team; // the team this piece plays for
    PieceType type; // Returns the type of this piece
    MoveType moveType; // Returns the move type of this piece
    int xPos; int yPos; // X and Y positions
    bool moved = false; // Whether piece has moved or not
    
    public:
    Piece(Team, int, int, PieceType, string); // Constructor
    virtual ~Piece() {} // Virtual destructor

    Team getTeam() const { return team; };
    PieceType getType() const { return type; }
    string getFileName() const { return filename; }
    bool hasMoved() const { return moved; }
    int getX() const { return xPos; }
    int getY() const { return yPos; }

    virtual moveTypes calcPossibleMoves(Piece*[8][8]) const = 0; // Pure virtual function
    void move(int x, int y) { if (xPos != x || yPos != y) { moved = true; xPos = x; yPos = y; } }
};
