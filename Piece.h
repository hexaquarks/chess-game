#pragma once
#include <utility>
#include <vector>
#include <tuple>
#include <iostream>

class King;

class Piece {
    public:
        enum Team { WHITE, BLACK, NONE }; 
        enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY }; 
        enum MoveType { NORMAL, CASTLE, ENPASSANT, NEWPIECE, INIT }; 
        

        Team getTeam() { return _team; };

        // Constructor
        Piece(Team team, std::pair<int,int> pos, PieceType type);

        // Destructor
        ~Piece();

    public: 
        std::string _filename;  // the filename for this piece
        Team _team; // the team this piece plays for
        PieceType _type; // Returns the type of this piece
        MoveType _moveType; // Returns the move type of this piece
        std::pair<int, int> _pos; // The position of this piece
        std::vector<std::tuple <int, int, Piece::MoveType>> _possibleMoves; // list of all possible moves for this piece
};