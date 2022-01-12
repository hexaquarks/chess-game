#include "King.h"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k") {}

moveTypes King::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int x = getX(), y = getY();

    for (int i = max(0, x-1); i <= min(7, x+1); ++i) {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            if (board[i][j] == nullptr || board[i][j]->getTeam() != getTeam()) {
                Piece* previous = board[i][j];

                board[x][y] = nullptr; board[i][j] = (Piece*) this; // Move king to test
                bool checked = isChecked(board, i, j); // Check if king is checked in this position
                board[x][y] = (Piece*) this; board[i][j] = previous; // Put king back

                // If king is not checked, we can add move
                if (!checked) {
                    if (board[i][j] == nullptr) moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
                    else moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
                }
            }
        }
    }
    return moves;
}

moveTypes King::possibleMovesNoCheck(Piece* board[8][8]) const {
    moveTypes moves;
    int x = getX(), y = getY();

    for (int i = max(0, x-1); i <= min(7, x+1); ++i) {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            if (board[i][j] == nullptr || board[i][j]->getTeam() != getTeam()) {
                if (board[i][j] == nullptr) moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
                else moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
            }
        }
    }
    return moves;
}

bool King::isChecked(Piece* board[8][8], int i, int j) const {
    // Looping through every piece
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = board[row][col];

            // If piece has opposite colour, it is a potential danger
            if (p != nullptr && p->getTeam() != getTeam()) {
                moveTypes positions = (p->getType() == PieceType::KING)? ((King*) p)->possibleMovesNoCheck(board): p->calcPossibleMoves(board);

                // Loop through every possible move to see if king is in danger or not
                for (auto& move: positions)
                    if (get<0>(move).first == i && get<0>(move).second == j)
                        return true;
            }
        }
    }

    // No checks found
    return false;
}
