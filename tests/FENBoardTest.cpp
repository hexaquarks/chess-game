#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BoardFEN
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../include/Logic/Board.hpp"
#include "BoardPositionsUtil.hpp"

BOOST_AUTO_TEST_CASE(TestStartPosition)
{
    Board board(testUtil::FEN_DEFAULT_POSITION);

    BOOST_CHECK_EQUAL(board.getBoardTile({'a', 2})->getType(), PieceType::PAWN);
    BOOST_CHECK_EQUAL(board.getBoardTile({'h', 8})->getType(), PieceType::ROOK);
}

BOOST_AUTO_TEST_CASE(TestCustomPosition)
{
    Board board(testUtil::FEN_SCOTCH_MAINLINE);

    BOOST_CHECK_EQUAL(board.getBoardTile({'d', 4})->getType(), PieceType::KNIGHT);
    BOOST_CHECK_EQUAL(board.getBoardTile({'f', 6})->getType(), PieceType::QUEEN);
    BOOST_CHECK_EQUAL(board.getBoardTile({'c', 5})->getType(), PieceType::BISHOP);
}

