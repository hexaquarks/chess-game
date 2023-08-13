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
    m_blackKing = std::make_shared<King>(Team::BLACK, 4, 0);
    m_whiteKing = std::make_shared<King>(Team::WHITE, 4, 7);

    // Set first row (black)
    m_board[0][0] = std::make_shared<Rook>(Team::BLACK, 0, 0);
    m_board[0][1] = std::make_shared<Knight>(Team::BLACK, 1, 0);
    m_board[0][2] = std::make_shared<Bishop>(Team::BLACK, 2, 0);
    m_board[0][3] = std::make_shared<Queen>(Team::BLACK, 3, 0);
    m_board[0][4] = m_blackKing;
    m_board[0][5] = std::make_shared<Bishop>(Team::BLACK, 5, 0);
    m_board[0][6] = std::make_shared<Knight>(Team::BLACK, 6, 0);
    m_board[0][7] = std::make_shared<Rook>(Team::BLACK, 7, 0);

    // Set last row (white)
    m_board[7][0] = std::make_shared<Rook>(Team::WHITE, 0, 7);
    m_board[7][1] = std::make_shared<Knight>(Team::WHITE, 1, 7);
    m_board[7][2] = std::make_shared<Bishop>(Team::WHITE, 2, 7);
    m_board[7][3] = std::make_shared<Queen>(Team::WHITE, 3, 7);
    m_board[7][4] = m_whiteKing;
    m_board[7][5] = std::make_shared<Bishop>(Team::WHITE, 5, 7);
    m_board[7][6] = std::make_shared<Knight>(Team::WHITE, 6, 7);
    m_board[7][7] = std::make_shared<Rook>(Team::WHITE, 7, 7);

    // Fill in white and black pawns
    for (size_t file = 0; file < 8; ++file)
    {
        m_board[1][file] = std::make_shared<Pawn>(Team::BLACK, file, 1); // Black pawn on row 1
        m_board[6][file] = std::make_shared<Pawn>(Team::WHITE, file, 6); // White pawn on row 6
    }

    // Add every black piece to the list of black pieces
    for (size_t rank = 0; rank < 2; ++rank)
        for (size_t file = 0; file < 8; ++file)
            m_blackPieces.push_back(m_board[rank][file]);

    // Add every white piece to the list of white pieces
    for (size_t rank = 6; rank < 8; ++rank)
        for (size_t file = 0; file < 8; ++file)
            m_whitePieces.push_back(m_board[rank][file]);

    // Delete pieces that have already moved
    for (size_t rank = 0; rank < 8; ++rank)
        for (size_t file = 2; file < 6; ++file)
            m_board[file][rank].reset();

    m_turn = Team::WHITE; // Reset the first move to be for white
    m_pLastMovedPiece.reset();
    setIsKingChecked(false);
    m_isFlipped = false;
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
                case 'r': m_board[rowIndex][colIndex] = std::make_shared<Rook>(Team::BLACK, colIndex, rowIndex); break;
                case 'n': m_board[rowIndex][colIndex] = std::make_shared<Knight>(Team::BLACK, colIndex, rowIndex); break;
                case 'b': m_board[rowIndex][colIndex] = std::make_shared<Bishop>(Team::BLACK, colIndex, rowIndex); break;
                case 'q': m_board[rowIndex][colIndex] = std::make_shared<Queen>(Team::BLACK, colIndex, rowIndex); break;
                case 'k': 
                {
                    m_blackKing = std::make_shared<King>(Team::BLACK, colIndex, rowIndex); 
                    m_board[rowIndex][colIndex] = m_blackKing;
                    break;
                }
                case 'p': m_board[rowIndex][colIndex] = std::make_shared<Pawn>(Team::BLACK, colIndex, rowIndex); break;
                case 'R': m_board[rowIndex][colIndex] = std::make_shared<Rook>(Team::WHITE, colIndex, rowIndex); break;
                case 'N': m_board[rowIndex][colIndex] = std::make_shared<Knight>(Team::WHITE, colIndex, rowIndex); break;
                case 'B': m_board[rowIndex][colIndex] = std::make_shared<Bishop>(Team::WHITE, colIndex, rowIndex); break;
                case 'Q': m_board[rowIndex][colIndex] = std::make_shared<Queen>(Team::WHITE, colIndex, rowIndex); break;
                case 'K': 
                {
                    m_whiteKing = std::make_shared<King>(Team::WHITE, colIndex, rowIndex);
                    m_board[rowIndex][colIndex] = m_whiteKing;
                    break;
                }
                case 'P': m_board[rowIndex][colIndex] = std::make_shared<Pawn>(Team::WHITE, colIndex, rowIndex); break;
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

std::shared_ptr<Piece>& Board::getBoardTile(const std::pair<char, int>& coord_ /* <file, rank> : {'a', 2} */)  
{
    assert(coord_.second >= 1 && coord_.second <= 8);
    assert(coord_.first >= 'a' && coord_.first <= 'h');

    return m_board[8 - coord_.second][static_cast<int>(coord_.first - 'a')];
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
    auto playerPieces = (m_turn == Team::WHITE)? m_whitePieces: m_blackPieces;

    for (auto& piece: playerPieces)
    {
        if (piece->isCached()) continue;

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
        const auto [file, rank] = (*it).getTarget();

        // Store piece occupied by target square
        std::shared_ptr<Piece> temp = getBoardTile(file, rank);

        int initialFile = pSelectedPiece_->getFile();
        int initialRank = pSelectedPiece_->getRank();

        setBoardTile(file, rank, pSelectedPiece_, false); // Move this piece to target square
        resetBoardTile(initialFile, initialRank, false); // Set null to selected piece's square

        if (kingIsChecked()) it = possibleMoves_.erase(it);
        else ++it;

        setBoardTile(initialFile, initialRank, pSelectedPiece_, false);
        setBoardTile(file, rank, temp, false);
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
    int file_,
    int rank_ ) const
{
    for (auto& move : getAllCurrentlyAvailableMoves())
    {
        if (move.getSelectedPiece() == pSelectedPiece_)
        {
            if (move.getTarget().first == file_ && move.getTarget().second == rank_)
            {
                return move;
            }
        }
    }
    return std::nullopt;
}