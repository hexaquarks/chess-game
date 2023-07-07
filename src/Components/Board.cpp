#include "../../include/Components/Board.hpp"
#include "../../include/Pieces/Pawn.hpp"
#include "../../include/Pieces/Rook.hpp"
#include "../../include/Pieces/Piece.hpp"
#include "../../include/Pieces/Knight.hpp"
#include "../../include/Pieces/Bishop.hpp"
#include "../../include/Pieces/King.hpp"
#include "../../include/Pieces/Queen.hpp"
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
    m_blackKing = std::make_shared<King>(Team::BLACK, 0, 4);
    m_whiteKing = std::make_shared<King>(Team::WHITE, 7, 4);

    // Set first row (black)
    m_board[0][0] = std::make_shared<Rook>(Team::BLACK, 0, 0);
    m_board[0][1] = std::make_shared<Knight>(Team::BLACK, 0, 1);
    m_board[0][2] = std::make_shared<Bishop>(Team::BLACK, 0, 2);
    m_board[0][3] = std::make_shared<Queen>(Team::BLACK, 0, 3);
    m_board[0][4] = m_blackKing;
    m_board[0][5] = std::make_shared<Bishop>(Team::BLACK, 0, 5);
    m_board[0][6] = std::make_shared<Knight>(Team::BLACK, 0, 6);
    m_board[0][7] = std::make_shared<Rook>(Team::BLACK, 0, 7);

    // Set last row (white)
    m_board[7][0] = std::make_shared<Rook>(Team::WHITE, 7, 0);
    m_board[7][1] = std::make_shared<Knight>(Team::WHITE, 7, 1);
    m_board[7][2] = std::make_shared<Bishop>(Team::WHITE, 7, 2);
    m_board[7][3] = std::make_shared<Queen>(Team::WHITE, 7, 3);
    m_board[7][4] = m_whiteKing;
    m_board[7][5] = std::make_shared<Bishop>(Team::WHITE, 7, 5);
    m_board[7][6] = std::make_shared<Knight>(Team::WHITE, 7, 6);
    m_board[7][7] = std::make_shared<Rook>(Team::WHITE, 7, 7);

    // Fill in white and black pawns
    for (size_t col = 0; col < 8; ++col)
    {
        m_board[1][col] = std::make_shared<Pawn>(Team::BLACK, 1, col); // Black pawn on row 1
        m_board[6][col] = std::make_shared<Pawn>(Team::WHITE, 6, col); // White pawn on row 6
    }

    // Add every black piece to the list of black pieces
    for (size_t row = 0; row < 2; ++row)
        for (size_t col = 0; col < 8; ++col)
            m_blackPieces.push_back(m_board[row][col]);

    // Add every white piece to the list of white pieces
    for (size_t row = 6; row < 8; ++row)
        for (size_t col = 0; col < 8; ++col)
            m_whitePieces.push_back(m_board[row][col]);

    // Delete pieces that have already moved
    for (size_t row = 0; row < 8; ++row)
        for (size_t col = 2; col < 6; ++col)
            m_board[col][row].reset();

    m_turn = Team::WHITE; // Reset the first move to be for white
    m_pLastMovedPiece.reset();
    setIsKingChecked(false);
}

std::shared_ptr<Move> Board::applyMoveOnBoard(
    const std::optional<Move>& pSelectedMoveOpt_,
    coor2d currPos_,
    coor2d initialPos_,
    const std::shared_ptr<Piece>& pSelectedPiece_,
    const std::vector<Arrow>& arrows_)
{
    MoveType type = pSelectedMoveOpt_->getMoveType();
    auto pMove = std::make_shared<Move>(
        std::move(currPos_), 
        std::move(initialPos_), 
        pSelectedPiece_, 
        type);

    pMove->setCapturedPiece(getLastMovedPiece());
    pMove->setMoveArrows(arrows_);

    return pMove; 
}

void Board::updateBoardInfosAfterNewMove(
    const std::shared_ptr<Piece>& pSelectedPiece_,
    const std::shared_ptr<Move>& pMove_)
{
    setLastMovedPiece(pSelectedPiece_);
    setLastMoveType(pMove_->getMoveType());
    Piece::setLastMovedPiece(getLastMovedPiece());
    switchTurn();
    updateAllCurrentlyAvailableMoves();
    
    // It's better to have an explicit setter for this boolean instead
    // of invoking getAllCUrrentlyAvailableMoves.empty() in the getter 
    // because this function is suffiently expensive.
    setAreThereNoMovesAvailableAtCurrentPosition(
        getAllCurrentlyAvailableMoves().empty()
    );
    if (areThereNoMovesAvailableAtCurrentPosition()) pMove_->setNoMovesAvailable();

    setIsKingChecked(kingIsChecked());
    if (kingIsChecked()) pMove_->setChecked();
}

std::vector<Move> Board::possibleMovesFor(const std::shared_ptr<Piece>& piece)
{
    return piece->calcPossibleMoves(*this);
}

bool Board::kingIsChecked()
{
    return getKing()->isChecked(*this);
}

void Board::addPiece(const std::shared_ptr<Piece>& pPiece_)
{
    (pPiece_->getTeam() == Team::WHITE)? m_whitePieces.push_back(pPiece_)
                                       : m_blackPieces.push_back(pPiece_);
}

const std::shared_ptr<King>& Board::getKing() const
{
    return (m_turn == Team::WHITE)? m_whiteKing: m_blackKing;
}

void Board::setKingAsFirstMovement() 
{
    auto& king = (m_turn == Team::WHITE)? m_whiteKing: m_blackKing;
    king->setAsFirstMovement();
}

void Board::setBoardTile(int x_, int y_, std::shared_ptr<Piece>& pPiece_, bool record_)
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

void Board::updateAllCurrentlyAvailableMoves()
{
    std::vector<Move> moves;
    std::vector<std::shared_ptr<Piece>> playerPieces = (m_turn == Team::WHITE)? m_whitePieces: m_blackPieces;

    for (auto& piece: playerPieces)
    {
        if (piece->getX() == -1 || piece->getY() == -1) continue;

        std::vector<Move> pieceMoves = possibleMovesFor(piece);
        removeIllegalMoves(pieceMoves, piece);
        for (const auto& move: pieceMoves) moves.push_back(move);
    }

    m_allCurrentlyAvailableMoves = moves;
}

void Board::removeIllegalMoves(std::vector<Move>& possibleMoves_, std::shared_ptr<Piece>& pSelectedPiece_)
{
    std::vector<Move>::const_iterator it = possibleMoves_.begin();

    while (it != possibleMoves_.end())
    {
        const auto [y, x] = (*it).getTarget();

        // Store piece occupied by target square
        std::shared_ptr<Piece> temp = getBoardTile(x, y);

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

void Board::checkIfMoveMakesKingChecked(const std::shared_ptr<Move>& move_)
{
    m_isKingChecked = move_ ? move_->kingIsChecked() : false;
    m_currentlyNoMovesAvailable = move_ ? move_->hasNoMovesAvailable() : false;
}

std::optional<Move> Board::findSelectedMove(const std::shared_ptr<Piece>& pSelectedPiece_, int xPos_, int yPos_) const
{
    for (auto& move : getAllCurrentlyAvailableMoves())
    {
        if (move.getSelectedPiece() == pSelectedPiece_)
        {
            if (move.getTarget().first == yPos_ && move.getTarget().second == xPos_)
            {
                return move;
            }
        }
    }
    return std::nullopt;
}