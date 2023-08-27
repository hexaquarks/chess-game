#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/Components/Board.hpp"
#include "../include/Pieces/Piece.hpp"
#include "../include/Utilities/MoveTreeDisplayHandler.hpp"
#include "../include/Utilities/MoveTreeManager.hpp"
#include "../include/Utilities/PGNParser.hpp"
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

BOOST_FIXTURE_TEST_SUITE(MoveDisplayGenerationTests, MoveFilterBoardFixture)

BOOST_AUTO_TEST_CASE(TestEmptyTree)
{
    initBoard(testUtil::FEN_DEFAULT_POSITION);
    BOOST_CHECK_EQUAL(m_board.getAllCurrentlyAvailableMoves().size(), 20);
}

BOOST_AUTO_TEST_CASE(TestBlackInitialNumberOfMovesAvailable)
{
    MoveTreeManager moveTreeManager{m_board};
    const std::string myPNG = "1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)";
    moveTreeManager.initializeMoveSequenceFromPNG(myPNG);
}

BOOST_AUTO_TEST_SUITE_END()