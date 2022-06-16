#include "../../include/Pieces/King.hpp"
#include "../../include/Components/Board.hpp"

King::King(Team team, int x, int y): Piece(team, x, y, PieceType::KING, "k")
{
}

vector<Move> King::calcPossibleMoves(Board& board_) const
{
    vector<Move> moves;
    int x = getX();
    int y = getY();
    coor2d kingCoor = {x, y};
    shared_ptr<Piece> kingPtr = board_.getBoardTile(y, x);

    // Checking castling
    if (canCastleKingSide(board_))
        moves.push_back(Move({x, 6}, kingCoor, kingPtr, MoveType::CASTLE_KINGSIDE));
    if (canCastleQueenSide(board_))
        moves.push_back(Move({x, 2}, kingCoor, kingPtr, MoveType::CASTLE_QUEENSIDE));

    for (int i = max(0, x-1); i <= min(7, x+1); ++i)
    {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j)
        {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            shared_ptr<Piece> piece = board_.getBoardTile(j, i);
            if (!piece || piece->getTeam() != getTeam())
            {
                // Move king to test
                board_.resetBoardTile(y, x, false);
                board_.setBoardTile(j, i, kingPtr, false);

                // Check if king is checked in this position
                bool checked = isChecked(board_);

                // Put king back
                board_.setBoardTile(y, x, kingPtr, false);
                board_.setBoardTile(j, i, piece, false);

                // If king is not checked, we can add move
                if (!checked)
                {
                    if (!piece) moves.push_back(Move({i, j}, kingCoor, kingPtr, MoveType::NORMAL));
                    else moves.push_back(Move({i, j}, kingCoor, kingPtr, MoveType::CAPTURE));
                }
            }
        }
    }
    return moves;
}

vector<Move> King::possibleMovesNoCheck(Board& board_) const {
    vector<Move> moves;
    int x = getX();
    int y = getY();
    coor2d kingCoor = {x, y};
    shared_ptr<Piece> kingPtr = board_.getBoardTile(y, x);

    for (int i = max(0, x-1); i <= min(7, x+1); ++i) {
        for (int j = max(0, y-1); j <= min(7, y+1); ++j) {
            // King cannot stay in same place
            if (x == i && y == j) continue;

            // If position is empty or piece on it is of the opposite colour
            if (!board_.getBoardTile(j, i) || board_.getBoardTile(j, i)->getTeam() != getTeam()) {
                if (!board_.getBoardTile(j, i)) moves.push_back(Move({i, j}, kingCoor, kingPtr, MoveType::NORMAL));
                else moves.push_back(Move({i, j}, kingCoor, kingPtr, MoveType::CAPTURE));
            }
        }
    }
    return moves;
}

bool King::isChecked(Board& board_) const {
    // Looping through every piece
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            shared_ptr<Piece> p = board_.getBoardTile(col, row);

            // If piece has opposite colour, it is a potential danger
            if (p && p->getTeam() != getTeam()) {
                vector<Move> positions = (p->getType() == PieceType::KING)?
                    ((King*) p.get())->possibleMovesNoCheck(board_): p->calcPossibleMoves(board_);

                // Loop through every possible move to see if king is in danger or not
                for (auto& move: positions)
                    if (move.getTarget().first == getX() && move.getTarget().second == getY())
                        return true;
            }
        }
    }

    // No checks found
    return false;
}

bool King::canCastleKingSide(Board& board_) const {
    int x = getX(), y = getY();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 7), forget it
    shared_ptr<Piece> rightCorner = board_.getBoardTile(7, rookRow);
    if (!rightCorner || rightCorner->getType() != PieceType::ROOK || rightCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || rightCorner->hasMoved() || isChecked(board_))
        return false;

    // If one of the two squares on the right are not empty, forget it
    if (board_.getBoardTile(y+1, x) || board_.getBoardTile(y+2, x))
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board_, x, y, x, y+1); // Move king one place to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, x, y+1, x, y+2); // Move king two places to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, x, y+2, x, y); // Put king back
    return !traversesCheck;
}

bool King::canCastleQueenSide(Board& board_) const {
    int x = getX(), y = getY();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 0), forget it
    shared_ptr<Piece> leftCorner = board_.getBoardTile(0, rookRow);
    if (!leftCorner || leftCorner->getType() != PieceType::ROOK || leftCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || leftCorner->hasMoved() || isChecked(board_))
        return false;

    // If one of the three squares on the right are not empty, forget it
    if (board_.getBoardTile(y-1, x) || board_.getBoardTile(y-2, x) || board_.getBoardTile(y-3, x))
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board_, x, y, x, y-1); // Move king one place to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, x, y-1, x, y-2); // Move king two places to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, x, y-2, x, y); // Put king back
    return !traversesCheck;
}
