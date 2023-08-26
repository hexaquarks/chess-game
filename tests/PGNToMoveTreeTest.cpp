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
    //initBoard(testUtil::FEN_DEFAULT_POSITION);
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

BOOST_AUTO_TEST_CASE(TestNormalMainLineScotchWithVariations)
{
    const std::string scotchClassicalPNGWithVariations = 
        "1. e4 e5 2. Nf3 (2. Bc4 d6 (2... Nc6 3. Bd5) 3. Bd5) (2. Qh5 g6 3. Qxe5+) 2... f5";
    m_manager.initializeMoveSequenceFromPNG(scotchClassicalPNGWithVariations);
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

// BOOST_AUTO_TEST_CASE(TestNormalMainLineScotchWithVariations)
// {
//     const std::string scotchClassicalPNGWithVariations = 
//         "1. e4 e5 2. Nf3 Nc6 3. d4 exd4 4. Nxd4 Bc5 (4... Nxd4 5. Qxd4) "
//         "5. Be3 Qf6 6. c3 Nge7 7. Bc4 (7. g3 d5 (7... h5 8. h3) 8. Bg2) "
//         "7... Ne5 8. Be2 Qg6";
//     m_manager.initializeMoveSequenceFromPNG(scotchClassicalPNGWithVariations);
//     const std::string expectedString = 
//         "===== Printing the move tree =====\n"
//         "(e,4)\n"
//         "+--- (e,5)\n"
//         "    +--- (f,3)\n"
//         "        +--- (c,6)\n"
//         "            +--- (d,4)\n"
//         "                +--- (d,4)\n"
//         "                    +--- (d,4)\n";
    
//     BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
// }

BOOST_AUTO_TEST_CASE(TestX)
{
    //initBoard(testUtil::FEN_DEFAULT_POSITION);
    const std::string anyPNG{"1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)"};
    m_manager.initializeMoveSequenceFromPNG(anyPNG);
    const std::string expectedString = 
        "===== Printing the move tree =====\n"
        "(e,4)\n"
        "+--- (e,5)\n"
        "    +--- (d,4)\n"
        "    +--- (f,3)\n"
        "    |    +--- (c,6)\n"
        "    |        +--- (h,3)\n"
        "    +--- (c,3)\n";
    
    BOOST_CHECK_EQUAL(m_manager.getMoves().printTreeGet(), expectedString);
}

BOOST_AUTO_TEST_CASE(TestBlackInitialNumberOfMovesAvailable)
{
    MoveTreeManager moveTreeManager{m_board};
    const std::string myPNG = "1. e4 e5 2. d4 (2. Nf3 Nc6 3. h3) (2. Nc3)";
    moveTreeManager.initializeMoveSequenceFromPNG(myPNG);
    moveTreeManager.getMoves().printTree();
}

BOOST_AUTO_TEST_SUITE_END()