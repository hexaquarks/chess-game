#include <gtest/gtest.h>
#include <memory>
#include "../include/GameThread.hpp"
#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"
#include "../include/Utilities/MoveList.hpp"
#include "../include/Utilities/Move.hpp"

TEST(BoardTests, PieceCaptureTest) 
{
    Board board;
    PieceTransition transitioningPiece;
    MoveList moveList{board, transitioningPiece};

    /*
     * Pawn capture
    */
    // Move the pieces 
    shared_ptr<Piece> whitePawn = board.getBoardTile(0, 6);
    shared_ptr<Piece> blackPawn = board.getBoardTile(1, 1);
    board.setBoardTile(0, 4, whitePawn);
    board.setBoardTile(1, 3, blackPawn);
    
    // Perform the move using the MoveList class's mechanics
    coor2d whitePawnPos = { whitePawn.get()->getX() / g_CELL_SIZE, whitePawn.get()->getY() / g_CELL_SIZE };
    coor2d blackPawnPos = { blackPawn.get()->getX() / g_CELL_SIZE, blackPawn.get()->getY() / g_CELL_SIZE };
    shared_ptr<Move> pMove = make_shared<Move>(whitePawnPos, blackPawnPos, whitePawn, MoveType::CAPTURE);
    std::vector<Arrow> temp{}; // empty arrow vector
    moveList.addMove(pMove, temp);

    // Verify that the black pawn has been captured
    ASSERT_EQ(nullptr, board.getBoardTile(0, 4));
    ASSERT_EQ(board.getBoardTile(1, 3), whitePawn); 
}