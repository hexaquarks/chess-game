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
        MoveTreeManager m_moveTreeManager{m_board};

        MoveFilterBoardFixture() = default;
        ~MoveFilterBoardFixture() = default;

        std::string getDisplayedGeneratedMoves(const std::string& inputPGNString_)
        {
            m_moveTreeManager.initializeMoveSequenceFromPNG(inputPGNString_);
            const MoveTree& moveTree = m_moveTreeManager.getMoves();

            MoveTreeDisplayHandler handler{moveTree};

            std::vector<MoveInfo> actualMovesInfo = handler.generateMoveInfo();
            return printMoveInfosGet(actualMovesInfo);
        }
    };
}

BOOST_FIXTURE_TEST_SUITE(MoveDisplayGenerationTests, MoveFilterBoardFixture)

BOOST_AUTO_TEST_CASE(TestEmptyTree)
{
    const std::string PGNString = "";
    const std::string expectedString = "";
    
    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestSimple)
{
    MoveTreeManager moveTreeManager{m_board};
    const std::string PGNString = "1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)";
    const std::string expectedString = 
        "1.e4 e5 2.d4\n"
        "+--- A) 2.Nf3 Nc6 3.h3\n"
        "+--- B) 2.Nc3";
    
    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestBlackSubvariations)
{
    const std::string PGNString = 
        "1. e4 e5 (1... d5 2. exd5 Nf6 (2... Qxd5 3. "
        "Qg4 Qxg2 (3... Nf6)) (2... Bf5 3. Bd3 Bxd3 (3... Bg4)))";
    const std::string expectedString = 
        "1.e4 e5\n"
        "+--- A) 1...d5 2.exd5 Nf6\n"
        "    +--- A1) 2...Qxd5 3.Qg4 Qxg2\n"
        "        +--- A1,1) 3...Nf6\n"
        "    +--- A2) 2...Bf5 3.Bd3 Bxd3\n"
        "        +--- A2,1) 3...Bg4";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestWhiteSubvariations)
{
    const std::string PGNString = 
        "1. e4 d5 2. exd5 (2. e5 Nf6 3. Bb5+ (3. exf6)) "
        "2... Qxd5 3. Qf3 (3. Bb5+) (3. Nf3 Qe5+ 4. Nxe5 (4. Be2))";
    const std::string expectedString = 
        "1.e4 d5 2.exd5\n"
        "+--- A) 2.e5 Nf6 3.Bb5+\n"
        "    +--- A1) 3.dxf6\n"
        "Qxd5 3.Qf3\n"
        "+--- B) 3.Bb5+\n"
        "+--- C) 3.Nf3 Qe5+ 4.Nxe5\n"
        "    +--- A1) 4.Be2";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestDeepNestedVariation)
{
    const std::string PGNString = 
        "1. e4 d5 2. exd5 (2. e5 f6 3. e6 (3. exf6 e6 "
        "(3... exf6 4. Qh5+ g6 (4... Kd7))))";
    const std::string expectedString = 
        "1.e4 d5 2.exd5\n"
        "+--- A) 2.e5 f6 3.e6\n"
        "    +--- A1) 3.exf6 e6\n"
        "        +--- A1,1) 3...exf6 4.Qh5+ g6\n"
        "            +--- A1,0,1) 4...Kd7";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}


BOOST_AUTO_TEST_SUITE_END()