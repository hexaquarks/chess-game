#include <boost/test/unit_test.hpp>
#include <iostream>


#include "../include/Pieces/Pawn.hpp"
#include "../include/Components/Board.hpp"

namespace
{
    const std::string FEN_PAWN_MOVES = "8/8/8/1pPp4/8/4p1pp/5P1P/8 w - - 0 1";


}

BOOST_AUTO_TEST_CASE(TestEnPassantMoves)
{
    Board board{FEN_PAWN_MOVES};
    int rank = 3;
    int file = 2;
    const coor2d pawnCoord{3, 2};
    Pawn pawn{Team::WHITE, 3, 2};
    auto pPawnPos = board.getBoardTile(file, rank);
    const coor2d leftPawnCoor{'b', 5};
    const coor2d rightPawnCoor{'d', 5};
    const coor2d mainPawnCoor{'c', 5};
    int positiveDirection = 1;

    std::vector<Move> expectedMoves{
        Move({rank + 1, file - 1}, pawnCoord, pPawnPos, MoveType::ENPASSANT, leftPiece),
        Move({rank + 1, file + 1}, pawnCoor, pPawnPos, MoveType::ENPASSANT, rightPiece)

    }
    std::vector<Move> actualMoves;
    pawn.generateEnPassantMoves(actualMoves, board, positiveDirection);
}

BOOST_AUTO_TEST_CASE(TestMoveForwardMoves)
{
    
}

BOOST_AUTO_TEST_CASE(TestCaptureMoves)
{
    
}