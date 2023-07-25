#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BoardFEN
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../include/Components/Board.hpp"

namespace
{
    const std::string FEN_DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string FEN_SCOTCH_MAINLINE = "r1b1k1nr/pppp1ppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R b KQkq - 0 6";
    const std::string FEN_FRIED_LIVER_ATTACK_FRITZ = "r1bqkb1r/ppp2ppp/5n2/3Pp1N1/2Bn4/8/PPPP1PPP/RNBQK2R w KQkq - 1 6";
}

BOOST_AUTO_TEST_CASE(TestStartPosition)
{
    Board board(FEN_DEFAULT_POSITION);

    BOOST_CHECK_EQUAL(board.getBoardTile({'a', 2})->getType(), PieceType::PAWN);
    BOOST_CHECK_EQUAL(board.getBoardTile({'h', 8})->getType(), PieceType::ROOK);
}

BOOST_AUTO_TEST_CASE(TestCustomPosition)
{
    Board board(FEN_SCOTCH_MAINLINE);

    BOOST_CHECK_EQUAL(board.getBoardTile({'d', 4})->getType(), PieceType::KNIGHT);
    BOOST_CHECK_EQUAL(board.getBoardTile({'f', 6})->getType(), PieceType::QUEEN);
    BOOST_CHECK_EQUAL(board.getBoardTile({'c', 5})->getType(), PieceType::BISHOP);
}

