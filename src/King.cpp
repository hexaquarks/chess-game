#include "../include/King.hpp"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k") {}

moveTypes King::calcPossibleMoves(Piece* board[8][8]) const {
    moveTypes moves;
    int x = getX();
    int y = getY();

    // Checking castling
    if (canCastleKingSide(board))
        moves.push_back(make_tuple(make_pair(x, 6), MoveType::CASTLE_KINGSIDE));
    if (canCastleQueenSide(board))
        moves.push_back(make_tuple(make_pair(x, 2), MoveType::CASTLE_QUEENSIDE));

    for (int i = max(0, x-1); i <= min(7, x+1); ++i) {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            Piece* piece = board[i][j];
            if (piece == nullptr || piece->getTeam() != getTeam()) {
                board[x][y] = nullptr; board[i][j] = (Piece*) this; // Move king to test
                bool checked = isChecked(board, i, j); // Check if king is checked in this position
                board[x][y] = (Piece*) this; board[i][j] = piece; // Put king back

                // If king is not checked, we can add move
                if (!checked) {
                    if (piece == nullptr) moves.push_back(make_tuple(make_pair(i, j), MoveType::NORMAL));
                    else moves.push_back(make_tuple(make_pair(i, j), MoveType::CAPTURE));
                }
            }
        }
    }
    return moves;
}

moveTypes King::possibleMovesNoCheck(Piece* board[8][8]) const {
    moveTypes moves;
    int x = getX();
    int y = getY();

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

bool King::canCastleKingSide(Piece* board[8][8]) const {
    int x = getX(), y = getY();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 7), forget it
    Piece* rightCorner = board[rookRow][7];
    if (rightCorner == nullptr || rightCorner->getType() != PieceType::ROOK || rightCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || rightCorner->hasMoved() || isChecked(board))
        return false;

    // If one of the two squares on the right are not empty, forget it
    if (board[x][y+1] != nullptr || board[x][y+2] != nullptr)
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board, x, y, x, y+1); // Move king one place to the left
    if (isChecked(board, x, y+1)) traversesCheck = true;

    swapPieces(board, x, y+1, x, y+2); // Move king two places to the left
    if (isChecked(board, x, y+2)) traversesCheck = true;

    swapPieces(board, x, y+2, x, y); // Put king back
    return !traversesCheck;
}

bool King::canCastleQueenSide(Piece* board[8][8]) const {
    int x = getX(), y = getY();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 0), forget it
    Piece* leftCorner = board[rookRow][0];
    if (leftCorner == nullptr || leftCorner->getType() != PieceType::ROOK || leftCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || leftCorner->hasMoved() || isChecked(board))
        return false;

    // If one of the three squares on the right are not empty, forget it
    if (board[x][y-1] != nullptr || board[x][y-2] != nullptr || board[x][y-3] != nullptr)
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board, x, y, x, y-1); // Move king one place to the left
    if (isChecked(board, x, y-1)) traversesCheck = true;

    swapPieces(board, x, y-1, x, y-2); // Move king two places to the left
    if (isChecked(board, x, y-2)) traversesCheck = true;

    swapPieces(board, x, y-2, x, y); // Put king back
    return !traversesCheck;
}
