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

BOOST_AUTO_TEST_CASE(TestSimple)
{
    MoveTreeManager moveTreeManager{m_board};
    const std::string PGNString = "1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)";
    moveTreeManager.initializeMoveSequenceFromPNG(PGNString);
    const std::string expectedString = 
        "1.e4 e5 2.d4\n"
        "+--- A) 2.Nf3 Nc6 3.h3\n"
        "+--- B) 2.Nc3";

    const MoveTree& moveTree = moveTreeManager.getMoves();
    MoveTreeDisplayHandler handler{moveTree};
    std::vector<MoveInfo> actualMovesInfo = handler.generateMoveInfo();
    BOOST_CHECK_EQUAL(printMoveInfosGet(actualMovesInfo), expectedString);
}

BOOST_AUTO_TEST_CASE(TestBlackSubvariations)
{
    MoveTreeManager moveTreeManager{m_board};
    const std::string PGNString = 
        "1. e4 e5 (1... d5 2. exd5 Nf6 (2... Qxd5 3. "
        "Qg4 Qxg2 (3... Nf6)) (2... Bf5 3. Bd3 Bxd3 (3... Bg4)))";
    moveTreeManager.initializeMoveSequenceFromPNG(PGNString);
    const std::string expectedString = 
        "1.e4 e5\n"
        "+--- A) 1...d5 2.exd5 Nf6\n"
        "    +--- A1) 2...Qxd5 3.Qg4 Qxg2\n"
        "        +--- A1,1) 3...Nf6\n"
        "    +--- A2) 2...Bf5 3.Bd3 Bxd3\n"
        "        +--- A2,1) 3...Bg4";

    const MoveTree& moveTree = moveTreeManager.getMoves();
    MoveTreeDisplayHandler handler{moveTree};
    std::vector<MoveInfo> actualMovesInfo = handler.generateMoveInfo();
    BOOST_CHECK_EQUAL(printMoveInfosGet(actualMovesInfo), expectedString);
}

BOOST_AUTO_TEST_SUITE_END()