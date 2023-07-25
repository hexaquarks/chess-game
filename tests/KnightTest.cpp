#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "../include/Pieces/Knight.hpp"
#include "../include/Components/Board.hpp"

namespace
{
    const std::string FEN_KNIGHT_MOVES = "8/8/8/5n2/4p3/6N1/4P3/8 w - - 0 1";

    const std::shared_ptr<Piece>& initTestedPiece(Board& board, char file, int rank, Team expectedTeam)
    {
        auto& pPiece = board.getBoardTile({file, rank});   
        BOOST_CHECK_EQUAL(pPiece->getTeam(), expectedTeam);
        return std::move(pPiece);
    }
}

// For reference moves are appended by looking at coordinates 
// from left to right : 
//    int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
//    int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};
BOOST_AUTO_TEST_CASE(TestAllMoves)
{
    Board board{FEN_KNIGHT_MOVES};    

    const auto& pWhiteKnight = initTestedPiece(board, 'g', 3, Team::WHITE);
    const auto& pBlackPawn = initTestedPiece(board, 'e', 4, Team::BLACK);
    const auto& pBlackKnight = initTestedPiece(board, 'f', 5, Team::BLACK);

    const Knight* pKnight = dynamic_cast<Knight*>(pWhiteKnight.get());
    assert(pKnight != nullptr);

    // Generate the en passant moves
    std::vector<Move> actualMoves = pKnight->calcPossibleMoves(board);

    int rank = 5, file = 6;
    const coor2d initialPos{rank, file};
    std::vector<Move> expectedMoves{
        Move({rank + 2, file + 1}, initialPos, pWhiteKnight, MoveType::NORMAL),
        Move({rank - 2, file + 1}, initialPos, pWhiteKnight, MoveType::NORMAL),
        Move({rank - 2, file - 1}, initialPos, pWhiteKnight, MoveType::CAPTURE, pBlackKnight),
        Move({rank - 1, file - 2}, initialPos, pWhiteKnight, MoveType::CAPTURE, pBlackPawn),
        Move({rank + 2, file - 1}, initialPos, pWhiteKnight, MoveType::NORMAL)
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(
        actualMoves.begin(), 
        actualMoves.end(),
        expectedMoves.begin(),
        expectedMoves.end());
    BOOST_CHECK(actualMoves.size() == expectedMoves.size());
}