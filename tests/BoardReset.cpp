#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/Application/GameThread.hpp"
#include "../include/Logic/Board.hpp"
#include "../include/Logic/Pieces/Piece.hpp"

BOOST_AUTO_TEST_CASE(BoardResetTest)
{
    Board board;

    board.reset();
    BOOST_CHECK_EQUAL(board.getTurn(), Team::WHITE);

    // There should be 16 pieces on each team
    size_t whitePiecesSize = board.getWhitePieces().size();
    size_t blackPiecesSize = board.getBlackPieces().size();
    BOOST_CHECK_EQUAL(whitePiecesSize, 16);
    BOOST_CHECK_EQUAL(blackPiecesSize, 16);

    // It should always be white's turn.
    bool isWhitesTurn = board.getTurn() == Team::WHITE;
    BOOST_CHECK(isWhitesTurn);

    // Verify that pieces are brought back to their initial square.
    auto& whitePawn = board.getBoardTile(0, 6);
    board.resetBoardTile(0, 6);
    board.setBoardTile(0, 4, whitePawn);
    BOOST_CHECK_EQUAL(board.getBoardTile(0, 4).get(), whitePawn.get());
    board.reset();
    BOOST_CHECK_EQUAL(board.getBoardTile(0, 4), nullptr);
    auto& whitePawnAfterReset = board.getBoardTile(0, 6);
    BOOST_CHECK_EQUAL(whitePawnAfterReset->getType(), PieceType::PAWN);
    BOOST_CHECK_EQUAL(whitePawnAfterReset->getTeam(), Team::WHITE);

    // Verify that the board is no longer flipped if it was before
    board.flipBoard();
    board.reset();
    BOOST_CHECK(!board.isFlipped());

    // The last moved piece should be reseted since it's redundant
    BOOST_CHECK(!board.getLastMovedPiece().get());
}
