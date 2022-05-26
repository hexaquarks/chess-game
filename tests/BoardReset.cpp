#include <gtest/gtest.h>
#include "../include/GameThread.hpp"
#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"


TEST(BoardTests, ResetTest)
{
    Board board;

    board.reset();
    ASSERT_EQ(board.getTurn(), Team::WHITE);

    size_t whitePiecesSize = board.getWhitePieces().size();
    ASSERT_EQ(whitePiecesSize, 16);

    size_t blackPiecesSize = board.getBlackPieces().size();
    ASSERT_EQ(blackPiecesSize, 16);

    // Verify that pieces are brought back to their initial square.
    shared_ptr<Piece> whitePawn = board.getBoardTile(0, 6);
    board.setBoardTile(0, 4, whitePawn);
    ASSERT_EQ(board.getBoardTile(0, 4), whitePawn);
    board.reset();
    ASSERT_EQ(board.getBoardTile(0, 4), nullptr);
}
