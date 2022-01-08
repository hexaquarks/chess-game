#pragma once
#include <utility>
#include <vector>
#include <tuple>
#include <iostream>
using namespace std;


enum class Team { WHITE, BLACK, NONE };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY };
enum class MoveType { NORMAL, CASTLE, ENPASSANT, NEWPIECE, INIT };


class Piece {
    string filename; // the filename for this piece
    Team team; // the team this piece plays for
    PieceType type; // Returns the type of this piece
    MoveType moveType; // Returns the move type of this piece
    int xPos; int yPos; // X and Y positions

    public:
    Piece(Team, int, int, PieceType, string); // Constructor
    virtual ~Piece() {} // Virtual destructor
    Team getTeam() const { return team; };
    string getFileName() const { return filename; }
    virtual void calcPossibleMoves(Piece*[8][8]) const {} // Virtual function
    void move(int x, int y) { xPos = x; yPos = y; }
};
