#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"
#include "../include/Pieces/King.hpp"
#include "BoardPositionsUtil.hpp"

namespace 
{
    struct MoveFilterBoardFixture 
    {
        Board m_board;
        
        MoveFilterBoardFixture() = default;
        ~MoveFilterBoardFixture() = default;

        void initBoard(const std::string& fen_) 
        {
            m_board = Board(fen_);
            if (m_board.getTurn() != Team::WHITE) m_board.switchTurn();
            m_board.updateAllCurrentlyAvailableMoves();
        }
    };
}

BOOST_FIXTURE_TEST_SUITE(MoveFilteringTests, MoveFilterBoardFixture)

BOOST_AUTO_TEST_CASE(TestInitialNumberOfMovesAvailable)
{
    initBoard(testUtil::FEN_DEFAULT_POSITION);
    BOOST_CHECK_EQUAL(m_board.getAllCurrentlyAvailableMoves().size(), 20);
}

BOOST_AUTO_TEST_CASE(TestBlackInitialNumberOfMovesAvailable)
{
    initBoard(testUtil::FEN_DEFAULT_POSITION);
    m_board.switchTurn();
    m_board.updateAllCurrentlyAvailableMoves();
    BOOST_CHECK_EQUAL(m_board.getAllCurrentlyAvailableMoves().size(), 20);
}

BOOST_AUTO_TEST_CASE(TestKingInCheck)
{
    initBoard(testUtil::FEN_KING_IN_CHECK_POSITION);
    m_board.switchTurn();
    m_board.updateAllCurrentlyAvailableMoves();

    // There should be only one legal move to get out of check.
    BOOST_CHECK_EQUAL(m_board.getAllCurrentlyAvailableMoves().size(), 1);
}

BOOST_AUTO_TEST_CASE(TestCheckmate)
{
    initBoard(testUtil::FEN_FOOLS_CHECKMATE_REVERSED);
    m_board.switchTurn();
    m_board.updateAllCurrentlyAvailableMoves();

    // There should be only one legal move to get out of check.
    BOOST_CHECK_EQUAL(m_board.getAllCurrentlyAvailableMoves().size(), 0);
}

BOOST_AUTO_TEST_CASE(TestStalemate)
{
    initBoard(testUtil::FEN_STALEMATE_POSITION);
    
    // There should be no legal moves left.
    BOOST_CHECK_EQUAL(m_board.getAllCurrentlyAvailableMoves().size(), 0);
}

BOOST_AUTO_TEST_CASE(TestRemovalOfIllegalMoves1)
{
    initBoard(testUtil::FEN_POSITION_WITH_ILLEGAL_MOVES_1);

    std::vector<Move> allMoves = m_board.getAllCurrentlyAvailableMoves();

    // Ensure that the illegal moves are filtered out
    auto& pBlackKing = m_board.getBoardTile({'e', 8});
    int rank = 0, file = 4;
    const coor2d initialPosition1{file, rank};
    Move illegalMove1({file + 1, rank}, initialPosition1, pBlackKing, MoveType::NORMAL);

    auto& pBlackPawn = m_board.getBoardTile({'f', 7});
    rank = 0, file = 5;
    const coor2d initialPosition2{file, rank};
    Move illegalMove2({file, rank + 1}, initialPosition2, pBlackPawn, MoveType::NORMAL);

    for (auto& move : allMoves)
    {
        BOOST_CHECK(move != illegalMove1 && move != illegalMove2);
    }
}

BOOST_AUTO_TEST_CASE(TestRemovalOfIllegalMoves2)
{
    initBoard(testUtil::FEN_POSITION_WITH_ILLEGAL_MOVES_2);

    std::vector<Move> allMoves = m_board.getAllCurrentlyAvailableMoves();

    // In this position king is in check, there is only one move available
    
    // Ensure that the illegal moves are filtered out
    auto& pBlackKing = m_board.getBoardTile({'e', 8});
    int rank = 0, file = 4;
    const coor2d initialPosition1{file, rank};
    Move illegalMove1({file + 1, rank}, initialPosition1, pBlackKing, MoveType::NORMAL);

    auto& pBlackPawn = m_board.getBoardTile({'f', 7});
    rank = 0, file = 5;
    const coor2d initialPosition2{file, rank};
    Move illegalMove2({file, rank + 1}, initialPosition2, pBlackPawn, MoveType::NORMAL);

    for (auto& move : allMoves)
    {
        BOOST_CHECK(move != illegalMove1 && move != illegalMove2);
    }
}

BOOST_AUTO_TEST_SUITE_END()