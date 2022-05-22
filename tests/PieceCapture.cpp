#include <gtest/gtest.h>
#include <memory>
#include "../include/GameThread.hpp"
#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"

TEST(BoardTests, PieceCaptureTest) 
{
    Board board;

    // Pawn capture
    shared_ptr<Piece> whitePawn = board.getBoardTile(7, 0);
    shared_ptr<Piece> blackPawn = board.getBoardTile(1, 1);
    board.setBoardTile(5, 0, whitePawn);
    board.setBoardTile(3, 1, blackPawn);
    board.setBoardTile(3, 1, whitePawn); // capture
    ASSERT_EQ(nullptr, board.getBoardTile(5, 0));
    ASSERT_EQ(board.getBoardTile(3, 1), whitePawn); 
}