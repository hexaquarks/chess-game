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
    struct PGNToMoveTreeFixture
    {
        Board m_board;
        MoveTreeManager m_manager{m_board};
        
        PGNToMoveTreeFixture() = default;
        ~PGNToMoveTreeFixture() = default;

        // void init(const std::string& fen_) 
        // {
        //     m_board = Board(fen_);
        //     if (m_board.getTurn() != Team::WHITE) m_board.switchTurn();
        //     m_board.updateAllCurrentlyAvailableMoves();
        // }
    };
}

BOOST_FIXTURE_TEST_SUITE(PGNToMoveTreeTests, PGNToMoveTreeFixture)

BOOST_AUTO_TEST_CASE(TestEmptyPGN)
{
    const std::string emptyPGN{""};
    m_manager.initializeMoveSequenceFromPNG(emptyPGN);
    const std::string expectedString{""};
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestNormalMainLineScotch)
{
    const std::string scotchClassicalPNG{"1. e4 e5 2. Nf3 Nc6 3. d4 exd4 4. Nxd4"};
    m_manager.initializeMoveSequenceFromPNG(scotchClassicalPNG);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "    +--- (f,3)\n"
        "        +--- (c,6)\n"
        "            +--- (d,4)\n"
        "                +--- (d,4)\n"
        "                    +--- (d,4)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestCastleKingSide)
{
    const std::string castleKingSidePGN = 
        "1. e4 e5 2. Bc4 Bc5 3. Nf3 Nf6 4. O-O O-O 5. Nxe5";
    m_manager.initializeMoveSequenceFromPNG(castleKingSidePGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "    +--- (c,4)\n"
        "        +--- (c,5)\n"
        "            +--- (f,3)\n"
        "                +--- (f,6)\n"
        "                    +--- (g,1)\n"
        "                        +--- (g,8)\n"
        "                            +--- (e,5)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestCastleQueenSide)
{
    const std::string castleQueenSidePGN = 
        "1. b3 b6 2. Bb2 Bb7 3. Nc3 Nc6 4. d4 d6 "
        "5. Qd3 Qd7 6. O-O-O O-O-O 7. d5";
    m_manager.initializeMoveSequenceFromPNG(castleQueenSidePGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(b,3)\n"
        "+--- (b,6)\n"
        "    +--- (b,2)\n"
        "        +--- (b,7)\n"
        "            +--- (c,3)\n"
        "                +--- (c,6)\n"
        "                    +--- (d,4)\n"
        "                        +--- (d,6)\n"
        "                            +--- (d,3)\n"
        "                                +--- (d,7)\n"
        "                                    +--- (c,1)\n"
        "                                        +--- (c,8)\n"
        "                                            +--- (d,5)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestCheckAndCaptures)
{
    const std::string checkAndCapturesPGN = 
        "1. e4 d5 2. exd5 f5 (2... e5 3. Qf3 (3. f3 Qh4+)) 3. Qh5+ g6";
    m_manager.initializeMoveSequenceFromPNG(checkAndCapturesPGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (d,5)\n"
        "    +--- (d,5)\n"
        "        +--- (f,5)\n"
        "        |    +--- (h,5)\n"
        "        |        +--- (g,6)\n"
        "        +--- (e,5)\n"
        "            +--- (f,3)\n"
        "            +--- (f,3)\n"
        "                +--- (h,4)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}


BOOST_AUTO_TEST_CASE(TestMultipleEndings)
{
    const std::string multipleEndingsPGN = 
        "1. e4 e5 2. Nf3 (2. d4 exd4 3. c3 (3. Nf3 Nc6) "
        "3... dxc3) (2. f4 d5) 2... Nf6";
    m_manager.initializeMoveSequenceFromPNG(multipleEndingsPGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "    +--- (f,3)\n"
        "    |    +--- (f,6)\n"
        "    +--- (d,4)\n"
        "    |    +--- (d,4)\n"
        "    |        +--- (c,3)\n"
        "    |        |    +--- (c,3)\n"
        "    |        +--- (f,3)\n"
        "    |            +--- (c,6)\n"
        "    +--- (f,4)\n"
        "        +--- (d,5)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestRandomVariationWithSubvariations)
{
    const std::string randomPGN = 
        "1. e4 e5 2. Nf3 (2. Bc4 d6 (2... Nc6 3. Bd5) 3. Bd5) (2. Qh5 g6 3. Qxe5+) 2... f5";
    m_manager.initializeMoveSequenceFromPNG(randomPGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "    +--- (f,3)\n"
        "    |    +--- (f,5)\n"
        "    +--- (c,4)\n"
        "    |    +--- (d,6)\n"
        "    |    |    +--- (d,5)\n"
        "    |    +--- (c,6)\n"
        "    |        +--- (d,5)\n"
        "    +--- (h,5)\n"
        "        +--- (g,6)\n"
        "            +--- (e,5)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);

}

BOOST_AUTO_TEST_CASE(TestKingsPawnWithSubvariations)
{
    const std::string scotchClassicalPNGWithVariations = 
        "1. e4 e5 2. Nf3 (2. Bc4 Nf6 (2... Nc6 3. Nf3 Bc5 "
        "(3... Nf6 4. d3)) 3. d3) (2. f4 exf4 3. Nf3 (3. Nc3 Qh4+)) 2... Nc6";
    m_manager.initializeMoveSequenceFromPNG(scotchClassicalPNGWithVariations);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "    +--- (f,3)\n"
        "    |    +--- (c,6)\n"
        "    +--- (c,4)\n"
        "    |    +--- (f,6)\n"
        "    |    |    +--- (d,3)\n"
        "    |    +--- (c,6)\n"
        "    |        +--- (f,3)\n"
        "    |            +--- (c,5)\n"
        "    |            +--- (f,6)\n"
        "    |                +--- (d,3)\n"
        "    +--- (f,4)\n"
        "        +--- (f,4)\n"
        "            +--- (f,3)\n"
        "            +--- (c,3)\n"
        "                +--- (h,4)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestScandinavianWithSubvariations)
{
    const std::string scandiPGN = 
        "1. e4 d5 2. exd5 (2. Nc3 d4 (2... dxe4 3. Nxe4)) "
        "(2. e5 c5) 2... Qxd5 (2... Nf6 3. Bb5+ (3. d4 Nxd5))";
    m_manager.initializeMoveSequenceFromPNG(scandiPGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (d,5)\n"
        "    +--- (d,5)\n"
        "    |    +--- (d,5)\n"
        "    |    +--- (f,6)\n"
        "    |        +--- (b,5)\n"
        "    |        +--- (d,4)\n"
        "    |            +--- (d,5)\n"
        "    +--- (c,3)\n"
        "    |    +--- (d,4)\n"
        "    |    +--- (e,4)\n"
        "    |        +--- (e,4)\n"
        "    +--- (e,5)\n"
        "        +--- (c,5)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestDeepNestedSubvariations)
{
    const std::string deepNestedPGN = 
        "1. e4 e5 (1... f5 2. exf5 (2. e5 e6 "
        "(2... d6 3. e6 (3. Bd3 d5 (3... c6))))) 2. Bc4";
    m_manager.initializeMoveSequenceFromPNG(deepNestedPGN);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "|    +--- (c,4)\n"
        "+--- (f,5)\n"
        "    +--- (f,5)\n"
        "    +--- (e,5)\n"
        "        +--- (e,6)\n"
        "        +--- (d,6)\n"
        "            +--- (e,6)\n"
        "            +--- (d,3)\n"
        "                +--- (d,5)\n"
        "                +--- (c,6)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestBlackInitialNumberOfMovesAvailable)
{
    MoveTreeManager moveTreeManager{m_board};
    const std::string myPNG = "1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)";
    moveTreeManager.initializeMoveSequenceFromPNG(myPNG);

    const std::string expectedString = "";
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_SUITE_END()