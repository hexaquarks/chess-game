#include "../../include/Pieces/Knight.hpp"
#include "../../include/Components/Board.hpp"

#include <vector>

Knight::Knight(Team team, int x, int y):
    Piece(team, x, y, PieceType::KNIGHT, "n")
{
}

vector<Move> Knight::calcPossibleMoves(Board& board_) const
{
    vector<Move> moves;
    int xPos = getX();
    int yPos = getY();

    // All possible moves
    int X[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int Y[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; ++i)
    {
        int x = xPos + X[i];
        int y = yPos + Y[i];

        if (x >= 0 && y >= 0 && x < 8 && y < 8)
        {
            shared_ptr<Piece> p = board_.getBoardTile(yPos, xPos);
            if (!board_.getBoardTile(y, x))
            {
                moves.push_back(Move({x, y}, {xPos, yPos}, p, MoveType::NORMAL));
            }
            else if (board_.getBoardTile(y, x)->getTeam() != getTeam())
            {
                moves.push_back(Move({x, y}, {xPos, yPos}, p, MoveType::CAPTURE));
            }
        }
    }

    return moves;
}
