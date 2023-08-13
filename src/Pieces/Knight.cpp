#include "../../include/Pieces/Knight.hpp"
#include "../../include/Components/Board.hpp"

#include <vector>

// Index-based constructor
Knight::Knight(Team team_, int rank_, int file_):
    Piece(team_, rank_, file_, PieceType::KNIGHT, "n")
{
}

// Real coordinates constructor 
Knight::Knight(Team team_, coor2dChar& coords_):
    Piece(team_, coords_.first, coords_.second, PieceType::KNIGHT, "n")
{
}

std::vector<Move> Knight::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    int rank = getRank();
    int file = getFile();

    // All possible moves
    int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; ++i)
    {
        int newRank = rank + dx[i];
        int newFile = file + dy[i];

        if (newRank >= 0 && newFile >= 0 && newRank < 8 && newFile < 8)
        {
            std::shared_ptr<Piece> p = board_.getBoardTile(file, rank);
            if (!board_.getBoardTile(newFile, newRank))
                moves.push_back(Move({newFile, newRank}, {file, rank}, p, MoveType::NORMAL));
            else if (board_.getBoardTile(newFile, newRank)->getTeam() != getTeam())
                moves.push_back(Move({newFile, newRank}, {file, rank}, p, MoveType::CAPTURE));
        }
    }

    return moves;
}
