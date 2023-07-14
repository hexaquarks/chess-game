#include "../../include/Components/Board.hpp"
#include "../../include/Pieces/Pawn.hpp"
#include "../../include/Pieces/Rook.hpp"
#include "../../include/Pieces/Piece.hpp"
#include "../../include/Pieces/Knight.hpp"
#include "../../include/Pieces/Bishop.hpp"
#include "../../include/Pieces/King.hpp"
#include "../../include/Pieces/Queen.hpp"
#include "../../include/Utilities/Move.hpp"

#include <algorithm>
#include <cctype>
#include <cassert>

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

Board::Board(const std::string& fen_)
{
    auto splitPosition = std::find_if(fen_.begin(), fen_.end(), ::isspace);
    std::string position(fen_.begin(), splitPosition);
    std::string remainingFEN(splitPosition, fen_.end());
    
    size_t rowIndex = 0, colIndex = 0;
    for (const char& ch : position) 
    {
        if (ch == '/') 
        {
            ++rowIndex;
            colIndex = 0;
        } 
        else if (std::isdigit(ch)) 
        {
            colIndex += ch - '0';
        } 
        else 
        {
            switch(ch) 
            {
                case 'r': m_board[rowIndex][colIndex] = std::make_shared<Rook>(Team::BLACK, rowIndex, colIndex); break;
                case 'n': m_board[rowIndex][colIndex] = std::make_shared<Knight>(Team::BLACK, rowIndex, colIndex); break;
                case 'b': m_board[rowIndex][colIndex] = std::make_shared<Bishop>(Team::BLACK, rowIndex, colIndex); break;
                case 'q': m_board[rowIndex][colIndex] = std::make_shared<Queen>(Team::BLACK, rowIndex, colIndex); break;
                case 'k': 
                {
                    m_board[rowIndex][colIndex] = std::make_shared<King>(Team::BLACK, rowIndex, colIndex); 
                    // m_blackKing = dynamic_cast<King*>(m_board[rowIndex][colIndex]); 
                    break;
                }
                case 'p': m_board[rowIndex][colIndex] = std::make_shared<Pawn>(Team::BLACK, rowIndex, colIndex); break;
                case 'R': m_board[rowIndex][colIndex] = std::make_shared<Rook>(Team::WHITE, rowIndex, colIndex); break;
                case 'N': m_board[rowIndex][colIndex] = std::make_shared<Knight>(Team::WHITE, rowIndex, colIndex); break;
                case 'B': m_board[rowIndex][colIndex] = std::make_shared<Bishop>(Team::WHITE, rowIndex, colIndex); break;
                case 'Q': m_board[rowIndex][colIndex] = std::make_shared<Queen>(Team::WHITE, rowIndex, colIndex); break;
                case 'K': 
                {
                    m_board[rowIndex][colIndex] = std::make_shared<King>(Team::WHITE, rowIndex, colIndex); 
                    // m_whiteKing = dynamic_cast<King*>(m_board[rowIndex][colIndex]); 
                    break;
                }
                case 'P': m_board[rowIndex][colIndex] = std::make_shared<Pawn>(Team::WHITE, rowIndex, colIndex); break;
                default: break;
            }
            if (m_board[rowIndex][colIndex]->getTeam() == Team::BLACK)
                m_blackPieces.push_back(m_board[rowIndex][colIndex]);
            else
                m_whitePieces.push_back(m_board[rowIndex][colIndex]);

            ++colIndex;
        }
    }

    m_turn = remainingFEN[1] == 'w' ? Team::WHITE : Team::BLACK;

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

std::shared_ptr<Piece>& Board::getBoardTile(const std::pair<char, char>& coord_ /* <file, rank> : {'a', 2} */)  
{
    assert(std::isalpha(coord_.first));
    assert(coord_.second >= '1' && coord_.second <= '8');
    assert(coord_.first >= 'a' && coord_.first <= 'h');

    return m_board[7 - static_cast<int>(coord_.second - '1')][static_cast<int>(coord_.first - 'a')];
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

void Board::setBoardTile(int file_, int rank_, std::shared_ptr<Piece>& pPiece_, bool record_)
{
    if (record_ && m_board[rank_][file_])
    {
        m_board[rank_][file_]->move(-1, -1);
    }
    m_board[rank_][file_] = pPiece_;
    if (pPiece_) pPiece_->move(rank_, file_, record_);
}

void Board::resetBoardTile(int file_, int rank_, bool record_)
{
    if (record_ && m_board[rank_][file_])
    {
        m_board[rank_][file_]->move(-1, -1);
    }
    m_board[rank_][file_].reset();
}

void Board::updateAllCurrentlyAvailableMoves()
{
    std::vector<Move> moves;
    std::vector<std::shared_ptr<Piece>> playerPieces = (m_turn == Team::WHITE)? m_whitePieces: m_blackPieces;

    for (auto& piece: playerPieces)
    {
        if (piece->getRank() == -1 || piece->getFile() == -1) continue;

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

        int initialX = pSelectedPiece_->getFile();
        int initialY = pSelectedPiece_->getRank();

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

std::optional<Move> Board::findSelectedMove(
    const std::shared_ptr<Piece>& pSelectedPiece_, 
    int rank_, 
    int file_) const
{
    for (auto& move : getAllCurrentlyAvailableMoves())
    {
        if (move.getSelectedPiece() == pSelectedPiece_)
        {
            if (move.getTarget().first == rank_ && move.getTarget().second == file_)
            {
                return move;
            }
        }
    }
    return std::nullopt;
}