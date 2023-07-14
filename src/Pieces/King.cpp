#include "../../include/Pieces/King.hpp"
#include "../../include/Components/Board.hpp"

King::King(Team team, int rank, int file): Piece(team, rank, file, PieceType::KING, "k")
{
}

std::vector<Move> King::calcPossibleMoves(Board& board_) const
{
    std::vector<Move> moves;
    int rank = getRank();
    int file = getFile();
    coor2d kingCoor = {rank, file};
    std::shared_ptr<Piece> kingPtr = board_.getBoardTile(file, rank);

    // Checking castling
    if (canCastleKingSide(board_))
        moves.push_back(Move({rank, 6}, kingCoor, kingPtr, MoveType::CASTLE_KINGSIDE));
    if (canCastleQueenSide(board_))
        moves.push_back(Move({rank, 2}, kingCoor, kingPtr, MoveType::CASTLE_QUEENSIDE));

    for (int i = std::max(0, rank-1); i <= std::min(7, rank+1); ++i)
    {
        for (int j = std::max(0, file-1); j <= std::min(7, file+1); ++j)
        {
            // King cannot stay in same place
            if (rank == i && file == j) continue;

            // If position is empty or piece on it is of the opposite colour
            std::shared_ptr<Piece> piece = board_.getBoardTile(j, i);
            if (!piece || piece->getTeam() != getTeam())
            {
                // Move king to test
                board_.resetBoardTile(file, rank, false);
                board_.setBoardTile(j, i, kingPtr, false);

                // Check if king is checked in this position
                bool checked = isChecked(board_);

                // Put king back
                board_.setBoardTile(file, rank, kingPtr, false);
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

std::vector<Move> King::possibleMovesNoCheck(Board& board_) const {
    std::vector<Move> moves;
    int rank = getRank();
    int file = getFile();
    coor2d kingCoor = {rank, file};
    std::shared_ptr<Piece> kingPtr = board_.getBoardTile(file, rank);

    for (int i = std::max(0, rank-1); i <= std::min(7, rank+1); ++i) {
        for (int j = std::max(0, file-1); j <= std::min(7, file+1); ++j) {
            // King cannot stay in same place
            if (rank == i && file == j) continue;

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
    // Looping through everfile piece
    for (size_t row = 0; row < 8; ++row) {
        for (size_t col = 0; col < 8; ++col) {
            std::shared_ptr<Piece> p = board_.getBoardTile(col, row);

            // If piece has opposite colour, it is a potential danger
            if (p && p->getTeam() != getTeam()) {
                std::vector<Move> positions = (p->getType() == PieceType::KING)?
                    ((King*) p.get())->possibleMovesNoCheck(board_): p->calcPossibleMoves(board_);

                // Loop through every possible move to see if king is in danger or not
                for (auto& move: positions)
                    if (move.getTarget().first == getRank() && move.getTarget().second == getFile())
                        return true;
            }
        }
    }

    // No checks found
    return false;
}

bool King::canCastleKingSide(Board& board_) const {
    int rank = getRank(), file = getFile();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 7), forget it
    std::shared_ptr<Piece> rightCorner = board_.getBoardTile(7, rookRow);
    if (!rightCorner || rightCorner->getType() != PieceType::ROOK || rightCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || rightCorner->hasMoved() || isChecked(board_))
        return false;

    // If one of the two squares on the right are not empty, forget it
    if (board_.getBoardTile(file+1, rank) || board_.getBoardTile(file+2, rank))
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board_, rank, file, rank, file+1); // Move king one place to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, rank, file+1, rank, file+2); // Move king two places to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, rank, file+2, rank, file); // Put king back
    return !traversesCheck;
}

bool King::canCastleQueenSide(Board& board_) const {
    int rank = getRank(), file = getFile();
    const int rookRow = (getTeam() == Team::WHITE)? 7: 0;

    // If rook is not at position (rookRow, 0), forget it
    std::shared_ptr<Piece> leftCorner = board_.getBoardTile(0, rookRow);
    if (!leftCorner || leftCorner->getType() != PieceType::ROOK || leftCorner->getTeam() != getTeam())
        return false;

    // If king has moved or rook has moved or king is checked, forget it
    if (hasMoved() || leftCorner->hasMoved() || isChecked(board_))
        return false;

    // If one of the three squares on the right are not empty, forget it
    if (board_.getBoardTile(file-1, rank) || board_.getBoardTile(file-2, rank) || board_.getBoardTile(file-3, rank))
        return false;

    // If we traverse a check, forget it
    bool traversesCheck = false;
    swapPieces(board_, rank, file, rank, file-1); // Move king one place to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, rank, file-1, rank, file-2); // Move king two places to the left
    if (isChecked(board_)) traversesCheck = true;

    swapPieces(board_, rank, file-2, rank, file); // Put king back
    return !traversesCheck;
}
