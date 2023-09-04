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
        PGNParser m_PGNParser{m_moveTreeManager};

        MoveFilterBoardFixture() = default;
        ~MoveFilterBoardFixture() = default;

        std::string getDisplayedGeneratedMoves(const std::string& inputPGNString_)
        {
            m_PGNParser.generatedMoveTreeFromPGNSequence(inputPGNString_);
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
    const std::string PGNString = "1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)";
    const std::string expectedString = 
        "===== Printing generated moves info =====\n"
        "1.e4 e5 2.d4\n"
        "    A) 2.Nf3 Nc6 3.h3\n"
        "    B) 2.Nc3";
    
    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestBlackSubvariations)
{
    const std::string PGNString = 
        "1. e4 e5 (1... d5 2. exd5 Nf6 (2... Qxd5 3. "
        "Qg4 Qxg2 (3... Nf6)) (2... Bf5 3. Bd3 Bxd3 (3... Bg4)))";
    const std::string expectedString = 
        "===== Printing generated moves info =====\n"
        "1.e4 e5\n"
        "    A) 1...d5 2.exd5 Nf6\n"
        "        A1) 2...Qxd5 3.Qg4 Qxg2\n"
        "            A1,1) 3...Nf6\n"
        "        A2) 2...Bf5 3.Bd3 Bxd3\n"
        "            A2,1) 3...Bg4";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestWhiteSubvariations)
{
    const std::string PGNString = 
        "1. e4 d5 2. exd5 (2. e5 Nf6 3. Bb5+ (3. exf6)) "
        "2... Qxd5 3. Qf3 (3. Bb5+) (3. Nf3 Qe5+ 4. Nxe5 (4. Be2))";
    const std::string expectedString = 
        "===== Printing the generated moves info =====\n"
        "1.e4 d5 2.exd5\n"
        "    A) 2.e5 Nf6 3.Bb5+\n"
        "        A1) 3.dxf6\n"
        "Qxd5 3.Qf3\n"
        "    A) 3.Bb5+\n"
        "    B) 3.Nf3 Qe5+ 4.Nxe5\n"
        "        B1) 4.Be2";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestDeepNestedVariation)
{
    const std::string PGNString = 
        "1. e4 d5 2. exd5 (2. e5 f6 3. e6 (3. exf6 e6 "
        "(3... exf6 4. Qh5+ g6 (4... Kd7))))";
    const std::string expectedString = 
        "===== Printing the generated moves info =====\n"
        "1.e4 d5 2.exd5\n"
        "    A) 2.e5 f6 3.e6\n"
        "        A1) 3.exf6 e6\n"
        "            A1,1) 3...exf6 4.Qh5+ g6\n"
        "                A1,1,1) 4...Kd7";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestBlackAndWhiteMixSubvariations)
{
    const std::string PGNString = 
        "1. Nf3 Nc6 (1... d5 2. Nd4 (2. d4 Nc6 (2... Nf6 3. e3)) "
        "(2. e3 Qd7 (2... Bg4 3. h3 Bxf3 (3... Bh5)))) 2. d4 d5 (2... e6)";
    const std::string expectedString = 
        "===== Printing the generated moves info =====\n"
        "1.Nf3 Nc6\n"
        "    A) 1...d5 2.Nd4\n"
        "        A1) 2.d4 Nc6\n"
        "            A1,1) 2...Nf6 3.e3\n"
        "        A2) 2.e3 Qd7\n"
        "            A2,1) 2...Bg4 3.h3 Bxf3\n"
        "                C2,1,1) 3...Bh5\n"
        "2.d4 d5\n"
        "    A) 2...e6";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestVariationWrapAroundASingleSubvariation)
{
    const std::string PGNString = 
        "1. e4 d5 (1... e5 2. Nf3 (2. Bc4 d6) 2... Nc6 3. d4 exd4)";
    const std::string expectedString =
        "===== Printing the generated moves info =====\n" 
        "1.e4 d5\n"
        "    A) 1...e5 2.Nf3\n"
        "        A1) 2.Bc4 d6\n"
        "    Nc6 3.d4 exd4";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestVariationWrapAroundAMultipleSubvariation)
{
    const std::string PGNString = 
        "1. e4 d5 (1... e5 2. Nf3 (2. Bc4 d6) (2. Qh5 g6 3. Qg5) "
        "(2. Nc3) (2. c3) (2. d4 exd4 3. Qxd4) 2... Nc6 3. d4 exd4)";
    const std::string expectedString =
        "===== Printing the generated moves info =====\n" 
        "1.e4 d5\n"
        "    A) 1...e5 2.Nf3\n"
        "        A1) 2.Bc4 d6\n"
        "        A2) 2.Qh5 g6 3.Qg5\n"
        "        A3) 2.Nc3\n"
        "        A4) 2.c3\n"
        "        A5) 2.d4 exd4 3.Qxd4\n"
        "    Nc6 3.d4 exd4";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestNestedSubvariationWraps)
{
    const std::string PGNString = 
        "1. Nc3 Nf6 (1... d5 2. d4 (2. Nb1 e5 3. d3 Nc6 "
        "(3... Bc5 4. Nf3 e4 (4... Nf6 5. Nxe5) 5. dxe4)) "
        "2... Nc6 3. Nf3) 2. e4 e5 3. d3";
    const std::string expectedString =
        "===== Printing the generated moves info =====\n" 
        "1.Nc3 Nf6\n"
        "    A) 1...d5 2.d4\n"
        "        A1) 2.Nb1 e5 3.d3 Nc6\n"
        "            A1,1) 3...Bc5 4.Nf3 e4\n"
        "                A1,1,1) 4...Nf6 5.Nxe5\n"
        "            5.dxe4\n"
        "    Nc6 3.Nf3\n"
        "2.e4 e5 3.d3";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestNestedMultiSubvariationWraps)
{
    const std::string PGNString = 
        "1. Nc3 Nf6 (1... d5 2. d4 (2. Nb1 e5 3. d3 Nc6 "
        "(3... Bc5 4. Nf3 e4 (4... Nf6 5. Nxe5) 5. dxe4) "
        "(3... Nf6 4. f4 (4. Nf3 Ne4) 4... exf4 5. e4)) "
        "2... Nc6 3. Nf3) 2. e4 e5 3. d3";
    const std::string expectedString =
        "===== Printing the generated moves info =====\n" 
        "1.Nc3 Nf6\n"
        "    A) 1...d5 2.d4\n"
        "        A1) 2.Nb1 e5 3.d3 Nc6\n"
        "            A1,1) 3...Bc5 4.Nf3 e4\n"
        "                A1,1,1) 4...Nf6 5.Nxe5\n"
        "            5.dxe4\n"
        "            A1,2) 3...Nf6 4.f4\n"
        "                A1,2,1) 4.Nf3 Ne4\n"
        "            exf4 5.e4\n"
        "    Nc6 3.Nf3\n"
        "2.e4 e5 3.d3";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_CASE(TestCastling)
{
    const std::string PGNString = 
        "1. e4 e5 2. Bd3 Bb4 3. Nf3 Nh6 4. O-O "
        "(4. Qe2 Qg5 5. Bc4 d6 6. b3 Be6 7. Bb2 "
        "Nc6 8. Nc3 O-O-O 9. O-O-O Kd7 10. Kb1) "
        "4... O-O 5. Nxe5 f6";
    const std::string expectedString =
        "===== Printing the generated moves info =====\n" 
        "1.e4 e5 2.Bd3 Bb4 3.Nf3 Nh6 4.O-O\n"
        "    A) 4.Qe2 Qg5 5.Bc4 d6 6.b3 Be6 7.Bb2 Nc6 8.Nc3 O-O-O 9.O-O-O Kd7 10.Kb1\n"
        "O-O 5.Nxe5 f6";

    BOOST_CHECK_EQUAL(getDisplayedGeneratedMoves(PGNString), expectedString);
}

BOOST_AUTO_TEST_SUITE_END()