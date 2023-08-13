#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../include/Pieces/Pawn.hpp"
#include "../include/Components/Board.hpp"
#include "BoardPositionsUtil.hpp"

namespace
{
    const int PAWN_DIR = -1; // White down black up.

    const std::shared_ptr<Piece>& initTestedPiece(Board& board, char file, int rank, Team expectedTeam)
    {
        auto& pPiece = board.getBoardTile({file, rank});   
        BOOST_CHECK_EQUAL(pPiece->getTeam(), expectedTeam);
        return std::move(pPiece);
    }

    void prepareEnPassantMove(
        Board& board,
        std::vector<Move>& actualMoves,
        const Pawn* pPawn, 
        const std::shared_ptr<Piece>& pBlackPawn)
    {
        // Generate the en passant moves
        pPawn->setLastMovedPiece(pBlackPawn);
        pBlackPawn->setLastMove(MoveType::INIT_SPECIAL);
        pPawn->generateEnPassantMoves(actualMoves, board, PAWN_DIR);
    }
}

BOOST_AUTO_TEST_CASE(TestEnPassantMoves)
{
    Board board{testUtil::FEN_PAWN_MOVES};    

    const auto& pWhitePawn = initTestedPiece(board, 'c', 5, Team::WHITE);
    const auto& pLeftBlackPawn = initTestedPiece(board, 'b', 5, Team::BLACK);
    const auto& pRightBlackPawn = initTestedPiece(board, 'd', 5, Team::BLACK);

    const Pawn* pPawn = dynamic_cast<Pawn*>(pWhitePawn.get());
    assert(pPawn != nullptr);

    std::vector<Move> actualMoves;

    // Generate the en passant moves
    prepareEnPassantMove(board, actualMoves, pPawn, pLeftBlackPawn);
    prepareEnPassantMove(board, actualMoves, pPawn, pRightBlackPawn);

    int rank = 3, file = 2;
    const coor2d initialPos{file, rank};
    std::vector<Move> expectedMoves{
        Move({file - 1, rank - 1}, initialPos, pWhitePawn, MoveType::ENPASSANT, pLeftBlackPawn),
        Move({file + 1, rank - 1}, initialPos, pWhitePawn, MoveType::ENPASSANT, pRightBlackPawn)
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actualMoves.begin(), 
        actualMoves.end(),
        expectedMoves.begin(),
        expectedMoves.end());
    BOOST_CHECK(actualMoves.size() == expectedMoves.size());
}

BOOST_AUTO_TEST_CASE(TestMoveForwardMoves)
{
    Board board{testUtil::FEN_PAWN_MOVES};

    const auto& pWhitePawnf2 = initTestedPiece(board, 'f', 2, Team::WHITE);
    const auto& pWhitePawnc5 = initTestedPiece(board, 'c', 5, Team::WHITE);

    const Pawn* pPawnf2 = dynamic_cast<Pawn*>(pWhitePawnf2.get());
    const Pawn* pPawnc5 = dynamic_cast<Pawn*>(pWhitePawnc5.get());
    assert(pPawnf2 != nullptr);
    assert(pPawnc5 != nullptr);

    std::vector<Move> actualMoves;
    pPawnf2->generateForwardMoves(actualMoves, board, PAWN_DIR);
    pPawnc5->generateForwardMoves(actualMoves, board, PAWN_DIR);

    int rankf2 = 6, filef2 = 5;
    const coor2d initialPosf2{filef2, rankf2};
    int rankc5 = 3, filec5 = 2;
    const coor2d initialPosc5{filec5, rankc5};

    std::vector<Move> expectedMoves{
        Move({filef2, rankf2 - 1}, initialPosf2, pWhitePawnf2, MoveType::NORMAL),
        Move({filef2, rankf2 - 2}, initialPosf2, pWhitePawnf2, MoveType::INIT_SPECIAL),
        Move({filec5, rankc5 - 1}, initialPosc5, pWhitePawnc5, MoveType::NORMAL)
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actualMoves.begin(), 
        actualMoves.end(),
        expectedMoves.begin(),
        expectedMoves.end());
    BOOST_CHECK(actualMoves.size() == expectedMoves.size());
}

BOOST_AUTO_TEST_CASE(TestCaptureMoves)
{
    Board board{testUtil::FEN_PAWN_MOVES};

    const auto& pWhitePawnf2 = initTestedPiece(board, 'f', 2, Team::WHITE);
    const auto& pWhitePawnh2 = initTestedPiece(board, 'h', 2, Team::WHITE);

    const Pawn* pPawnf2 = dynamic_cast<Pawn*>(pWhitePawnf2.get());
    const Pawn* pPawnh2 = dynamic_cast<Pawn*>(pWhitePawnh2.get());
    assert(pPawnf2 != nullptr);
    assert(pPawnh2 != nullptr);

    std::vector<Move> actualMoves;
    pPawnf2->generateCaptureMoves(actualMoves, board, PAWN_DIR);
    pPawnh2->generateCaptureMoves(actualMoves, board, PAWN_DIR);

    int rankf2 = 6, filef2 = 5;
    const coor2d initialPosf2{filef2, rankf2};
    int rankh2 = 6, fileh2 = 7;
    const coor2d initialPosh2{fileh2, rankh2};

    // Order matters lol. Right is calulated before left captures.
    std::vector<Move> expectedMoves{
        Move({filef2 + 1, rankf2 - 1}, initialPosf2, pWhitePawnf2, MoveType::CAPTURE),
        Move({filef2 - 1, rankf2 - 1}, initialPosf2, pWhitePawnf2, MoveType::CAPTURE),
        Move({fileh2 - 1, rankh2 - 1}, initialPosh2, pWhitePawnh2, MoveType::CAPTURE)
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actualMoves.begin(), 
        actualMoves.end(),
        expectedMoves.begin(),
        expectedMoves.end());
    BOOST_CHECK(actualMoves.size() == expectedMoves.size()); 
}