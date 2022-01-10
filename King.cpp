#include "King.h"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k") {}

moveTypes King::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int x = getX();
    int y = getY();

    for (int i = max(0, x-1); i < min(8, x+1); ++i) {
        for (int j = max(0, y-1); j < min(8, y+1); ++i) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            if (board[i][j] == nullptr || board[i][j]->getTeam() != getTeam()) {
                bool isChecked = false;

                // Looping through every piece
                for (int row = 0; row < 8; ++row) {
                    for (int col = 0; col < 8; ++col) {
                        // If we already found a check, stop search
                        if (isChecked) break;
                        Piece* p = board[row][col];

                        // If piece has opposite colour, it is a potential danger
                        if (p != nullptr && p->getTeam() != getTeam()) {
                            moveTypes positions = p->calcPossibleMoves(board);

                            // Loop through every possible move to see if king is in danger or not
                            for (auto& move: positions) {
                                if (get<0>(move).first == i && get<0>(move).second == j) {
                                    isChecked = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                // If king is not checked, we can add move it
                if (!isChecked) {
                    if (board[i][j] == nullptr) moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
                    else moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
                }
            }
        }
    }
    return moves;
}
