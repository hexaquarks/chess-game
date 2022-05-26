#include "../../include/Components/Board.hpp"
#include "../../include/Pieces/Pawn.hpp"
#include "../../include/Pieces/Rook.hpp"
#include "../../include/Pieces/Knight.hpp"
#include "../../include/Pieces/Bishop.hpp"
#include "../../include/Pieces/King.hpp"
#include "../../include/Pieces/Queen.hpp"
#include "../../include/Pieces/Piece.hpp"
#include "../../include/Utilities/Move.hpp"

Board::Board(): m_turn(Team::WHITE)
{
    reset();
}

void Board::reset()
{
    // Reset piece lists
    m_whitePieces.clear();
    m_blackPieces.clear();

    // Set the kings
    m_blackKing = make_shared<King>(Team::BLACK, 0, 4);
    m_whiteKing = make_shared<King>(Team::WHITE, 7, 4);

    // Set first row (black)
    m_board[0][0] = make_shared<Rook>(Team::BLACK, 0, 0);
    m_board[0][1] = make_shared<Knight>(Team::BLACK, 0, 1);
    m_board[0][2] = make_shared<Bishop>(Team::BLACK, 0, 2);
    m_board[0][3] = make_shared<Queen>(Team::BLACK, 0, 3);
    m_board[0][4] = m_blackKing;
    m_board[0][5] = make_shared<Bishop>(Team::BLACK, 0, 5);
    m_board[0][6] = make_shared<Knight>(Team::BLACK, 0, 6);
    m_board[0][7] = make_shared<Rook>(Team::BLACK, 0, 7);

    // Set last row (white)
    m_board[7][0] = make_shared<Rook>(Team::WHITE, 7, 0);
    m_board[7][1] = make_shared<Knight>(Team::WHITE, 7, 1);
    m_board[7][2] = make_shared<Bishop>(Team::WHITE, 7, 2);
    m_board[7][3] = make_shared<Queen>(Team::WHITE, 7, 3);
    m_board[7][4] = m_whiteKing;
    m_board[7][5] = make_shared<Bishop>(Team::WHITE, 7, 5);
    m_board[7][6] = make_shared<Knight>(Team::WHITE, 7, 6);
    m_board[7][7] = make_shared<Rook>(Team::WHITE, 7, 7);

    // Fill in white and black pawns
    for (uint8_t col = 0; col < 8; ++col)
    {
        m_board[1][col] = make_shared<Pawn>(Team::BLACK, 1, col); // Black pawn on row 1
        m_board[6][col] = make_shared<Pawn>(Team::WHITE, 6, col); // White pawn on row 6
    }

    // Add every black piece to the list of black pieces
    for (uint8_t row = 0; row < 2; ++row)
        for (uint8_t col = 0; col < 8; ++col)
            m_blackPieces.push_back(m_board[row][col]);

    // Add every white piece to the list of white pieces
    for (uint8_t row = 6; row < 8; ++row)
        for (uint8_t col = 0; col < 8; ++col)
            m_whitePieces.push_back(m_board[row][col]);

    // Delete pieces that have already moved
    for (uint8_t row = 0; row < 8; ++row)
        for (uint8_t col = 2; col < 6; ++col)
            m_board[col][row].reset();

    m_turn = Team::WHITE; // Reset the first move to be for white
}

vector<Move> Board::possibleMovesFor(shared_ptr<Piece>& piece)
{
    return piece->calcPossibleMoves(*this);
}

bool Board::kingIsChecked()
{
    return getKing()->isChecked(*this);
}

void Board::addPiece(shared_ptr<Piece>& pPiece_)
{
    (pPiece_->getTeam() == Team::WHITE)? m_whitePieces.push_back(pPiece_)
                                       : m_blackPieces.push_back(pPiece_);
}

shared_ptr<King> Board::getKing() const
{
    return (m_turn == Team::WHITE)? m_whiteKing: m_blackKing;
}

void Board::setBoardTile(int x_, int y_, shared_ptr<Piece>& pPiece_, bool record_)
{
    if (record_ && m_board[y_][x_])
    {
        m_board[y_][x_]->move(-1, -1);
    }
    m_board[y_][x_] = pPiece_;
    if (pPiece_) pPiece_->move(y_, x_, record_);
}

void Board::resetBoardTile(int x_, int y_, bool record_)
{
    if (record_ && m_board[y_][x_])
    {
        m_board[y_][x_]->move(-1, -1);
    }
    m_board[y_][x_].reset();
}

vector<Move> Board::calculateAllMoves()
{
    vector<Move> moves;
    vector<shared_ptr<Piece>> playerPieces = (m_turn == Team::WHITE)? m_whitePieces: m_blackPieces;
    for (shared_ptr<Piece> piece: playerPieces)
    {
        if (piece->getX() == -1 || piece->getY() == -1) continue;
        vector<Move> pieceMoves = possibleMovesFor(piece);
        removeIllegalMoves(pieceMoves, piece);
        for (auto& move: pieceMoves)
            moves.push_back(move);
    }
    return moves;
}

void Board::removeIllegalMoves(vector<Move>& possibleMoves_, shared_ptr<Piece>& pSelectedPiece_)
{
    vector<Move>::const_iterator it = possibleMoves_.begin();

    while (it != possibleMoves_.end())
    {
        int x = (*it).getTarget().second;
        int y = (*it).getTarget().first;

        // Store piece occupied by target square
        shared_ptr<Piece> temp = getBoardTile(x, y);

        int initialX = pSelectedPiece_->getY();
        int initialY = pSelectedPiece_->getX();

        setBoardTile(x, y, pSelectedPiece_, false); // Move this piece to target square
        resetBoardTile(initialX, initialY, false); // Set null to selected piece's square

        if (kingIsChecked()) it = possibleMoves_.erase(it);
        else ++it;

        setBoardTile(initialX, initialY, pSelectedPiece_, false);
        setBoardTile(x, y, temp, false);
    }
}

void Board::switchTurn()
{
    m_turn = (m_turn == Team::WHITE)? Team::BLACK: Team::WHITE;
}
