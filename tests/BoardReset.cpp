#include <gtest/gtest.h>
#include "../include/GameThread.hpp"
#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"


TEST(BoardResetTests, BoardReset)
{
    Board board;

    board.reset();
    ASSERT_EQ(board.getTurn(), Team::WHITE);

    size_t whitePiecesSize = board.getWhitePieces().size();
    ASSERT_EQ(whitePiecesSize, 16);

    size_t blackPiecesSize = board.getBlackPieces().size();
    ASSERT_EQ(blackPiecesSize, 16);
}
