#include "../../include/Pieces/King.hpp"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k") {}

vector<Move> King::calcPossibleMoves(Piece* board_[8][8]) const {
    vector<Move> moves;
    int x = getX();
    int y = getY();
    coor2d kingCoor = {x, y};
    Piece* pKingPos = board_[x][y];

    // Checking castling
    if (canCastleKingSide(board_))
        moves.push_back(Move(make_pair(x, 6), kingCoor, pKingPos, MoveType::CASTLE_KINGSIDE));
    if (canCastleQueenSide(board_))
        moves.push_back(Move(make_pair(x, 2), kingCoor, pKingPos, MoveType::CASTLE_QUEENSIDE));

    for (int i = max(0, x-1); i <= min(7, x+1); ++i) {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            Piece* piece = board_[i][j];
            if (piece == nullptr || piece->getTeam() != getTeam()) {
                board_[x][y] = nullptr; board_[i][j] = (Piece*) this; // Move king to test
                bool checked = isChecked(board_, i, j); // Check if king is checked in this position
                board_[x][y] = (Piece*) this; board_[i][j] = piece; // Put king back

                // If king is not checked, we can add move
                if (!checked) {
                    if (piece == nullptr) moves.push_back(Move(make_pair(i, j), kingCoor, pKingPos, MoveType::NORMAL));
                    else moves.push_back(Move(make_pair(i, j), kingCoor, pKingPos, MoveType::CAPTURE));
                }
            }
        }
    }
    return moves;
}

vector<Move> King::possibleMovesNoCheck(Piece* board_[8][8]) const {
    vector<Move> moves;
    int x = getX();
    int y = getY();
    coor2d kingCoor = {x, y};
    Piece* pKingPos = board_[x][y];

    for (int i = max(0, x-1); i <= min(7, x+1); ++i) {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            if (board_[i][j] == nullptr || board_[i][j]->getTeam() != getTeam()) {
                if (board_[i][j] == nullptr) moves.push_back(Move(make_pair(i, j), kingCoor, pKingPos, MoveType::NORMAL));
                else moves.push_back(Move(make_pair(i, j), kingCoor, pKingPos, MoveType::CAPTURE));
            }
        }
    }
    return moves;
}

bool King::isChecked(Piece* board_[8][8], int i, int j) const {
    // Looping through every piece
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            Piece* p = board_[row][col];

            // If piece has opposite colour, it is a potential danger
            if (p != nullptr && p->getTeam() != getTeam()) {
                vector<Move> positions = (p->getType() == PieceType::KING)
                    ? ((King*) p)->possibleMovesNoCheck(board_): p->calcPossibleMoves(board_);

                // Loop through every possible move to see if king is in danger or not
                for (auto& move: positions)
                    if (move.getTarget().first == i && move.getTarget().second == j)
                        return true;
            }
        }
    }

    // No checks found
    return false;
}

bool King::canCastleKingSide(Piece* board_[8][8]) const {
    int x = getX(), y = getY();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 7), forget it
    Piece* rightCorner = board_[rookRow][7];
    if (rightCorner == nullptr || rightCorner->getType() != PieceType::ROOK || rightCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || rightCorner->hasMoved() || isChecked(board_))
        return false;

    // If one of the two squares on the right are not empty, forget it
    if (board_[x][y+1] != nullptr || board_[x][y+2] != nullptr)
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board_, x, y, x, y+1); // Move king one place to the left
    if (isChecked(board_, x, y+1)) traversesCheck = true;

    swapPieces(board_, x, y+1, x, y+2); // Move king two places to the left
    if (isChecked(board_, x, y+2)) traversesCheck = true;

    swapPieces(board_, x, y+2, x, y); // Put king back
    return !traversesCheck;
}

bool King::canCastleQueenSide(Piece* board_[8][8]) const {
    int x = getX(), y = getY();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 0), forget it
    Piece* leftCorner = board_[rookRow][0];
    if (leftCorner == nullptr || leftCorner->getType() != PieceType::ROOK || leftCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || leftCorner->hasMoved() || isChecked(board_))
        return false;

    // If one of the three squares on the right are not empty, forget it
    if (board_[x][y-1] != nullptr || board_[x][y-2] != nullptr || board_[x][y-3] != nullptr)
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board_, x, y, x, y-1); // Move king one place to the left
    if (isChecked(board_, x, y-1)) traversesCheck = true;

    swapPieces(board_, x, y-1, x, y-2); // Move king two places to the left
    if (isChecked(board_, x, y-2)) traversesCheck = true;

    swapPieces(board_, x, y-2, x, y); // Put king back
    return !traversesCheck;
}
