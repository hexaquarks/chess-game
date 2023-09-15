#include "../../include/Utilities/MoveTreeManager.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/GameThread.hpp"

#include <cassert> 
#include <iterator>

MoveTreeManager::MoveTreeManager(Board& board_): m_board(board_)
{
}

bool MoveTreeManager::goToPreviousMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    if (!m_moveIterator.isAtTheBeginning())
    {
        undoMove(enableTransition_, arrowList_);
        m_board.switchTurn();
        m_board.updateAllCurrentlyAvailableMoves();
        
        return true;
    }
    return false;
}

bool MoveTreeManager::goToNextMove(
    bool enableTransition_, 
    const std::optional<size_t>& moveChildNumber_, 
    vector<Arrow>& arrowList_)
{
    if (!m_moveIterator.isAtTheEnd())
    {
        // Go to first children in the move list of the node, or at the specified index.
        m_moveIterator.goToChild(moveChildNumber_.value_or(0));

        applyMove(enableTransition_, arrowList_);
        m_board.switchTurn();
        m_board.updateAllCurrentlyAvailableMoves();

        return true;
    }
    return false;
}

void MoveTreeManager::addMove(const shared_ptr<Move>& move_, vector<Arrow>& arrowList_)
{
    applyMove(move_, true, true, arrowList_);
}

void MoveTreeManager::applyMove(
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    applyMove(m_moveIterator->m_move, false, enableTransition_, arrowList_);
}

void MoveTreeManager::applyMove(
    const shared_ptr<Move>& move_, 
    bool addToList_, 
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    if (!move_) return;
    
    const int castleRank = (m_board.getTurn() == Team::WHITE)? 7: 0;
    std::optional<shared_ptr<Piece>> pSecondPieceOpt;
    std::optional<shared_ptr<Piece>> pPromotingPieceOpt;
    shared_ptr<Piece> pSelectedPiece = move_->getSelectedPiece();
    shared_ptr<Piece> pCapturedPiece; // TODO change to optional. Requires some refactoring...
    int capturedFile = -1, capturedRank = -1;

    coor2d oldCoors;
    const auto [prevFile, prevRank] = move_->getInit();
    const auto [file, rank] = move_->getTarget();
    int secondFileInit = -1, secondFileTarget = -1;

    // Set the current tile of the piece null. Necessary for navigating back to current move through goToNextMove()
    m_board.resetBoardTile(prevFile, prevRank);
    arrowList_ = move_->getMoveArrows();

    switch (move_->getMoveType())
    {
        case MoveType::NORMAL:
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            // soundMove.play();
            break;

        case MoveType::CAPTURE:
        {
            pCapturedPiece = move_->getCapturedPiece();
            capturedFile = file;
            capturedRank = rank;
            auto pOldPiece = m_board.getBoardTile(file, rank);
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pOldPiece), m_moveIterator);
            }
            // soundCapture.play();
            break;
        }

        case MoveType::ENPASSANT:
            pCapturedPiece = move_->getCapturedPiece();
            capturedFile = pCapturedPiece->getFile();
            capturedRank = pCapturedPiece->getRank();
            oldCoors = {capturedFile, capturedRank};
            m_board.resetBoardTile(oldCoors.first, oldCoors.second);
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pCapturedPiece, oldCoors), m_moveIterator);
            }
            break;

        case MoveType::CASTLE_KINGSIDE:
        {
            secondFileInit = 7;
            secondFileTarget = 5;
            pSecondPieceOpt = m_board.getBoardTile(secondFileInit, castleRank);
            m_board.resetBoardTile(secondFileInit, castleRank);
            m_board.setBoardTile(secondFileTarget, castleRank, pSecondPieceOpt.value());
            m_board.setBoardTile(6, castleRank, pSelectedPiece);
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {6, castleRank};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPieceOpt.value()), m_moveIterator);
            }
            break;
        }

        case MoveType::CASTLE_QUEENSIDE:
        {
            secondFileInit = 0;
            secondFileTarget = 3;
            auto pSecondPiece = m_board.getBoardTile(secondFileInit, castleRank);
            m_board.resetBoardTile(secondFileInit, castleRank);
            m_board.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            m_board.setBoardTile(2, castleRank, pSelectedPiece);
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {2, castleRank};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPiece), m_moveIterator);
            }
            break;
        }

        case MoveType::INIT_SPECIAL:
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            break;

        case MoveType::NEWPIECE:
        {
            auto pOldPiece = m_board.getBoardTile(file, rank);
            pPromotingPieceOpt = make_shared<Queen>(pSelectedPiece->getTeam(), rank, file);
            Piece::setLastMovedPiece(pPromotingPieceOpt.value());
            m_board.setBoardTile(file, rank, pPromotingPieceOpt.value());
            m_board.addPiece(pPromotingPieceOpt.value());
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pOldPiece), m_moveIterator);
            }
            break;
        }
    }

    if (enableTransition_)
    {
        if (!addToList_ && pSelectedPiece)
        {
            // Enable piece visual transition
            setTransitioningPiece(
                false, pSelectedPiece, prevFile, prevRank, file, rank, pCapturedPiece,
                capturedFile, capturedRank
            );

            if (pSecondPieceOpt.has_value()) {
                setSecondTransitioningPiece(
                    pSecondPieceOpt.value(), secondFileInit, castleRank,
                    secondFileTarget, castleRank
                );
            }

            if (pPromotingPieceOpt.has_value()) {
                getTransitioningPiece().setPromotingPiece(pPromotingPieceOpt.value());
            }
        }
        else if (pSecondPieceOpt.has_value())
        {
            // Enable rook sliding when user just castled
            setTransitioningPiece(
                false, pSecondPieceOpt.value(), secondFileInit, castleRank, secondFileTarget, castleRank,
                pCapturedPiece, capturedFile, capturedRank
            );
        }
    }
}

void MoveTreeManager::undoMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    shared_ptr<Move> m = m_moveIterator->m_move;
    if (!m) return;

    shared_ptr<Piece> pCaptured = m->getCapturedPiece();
    shared_ptr<Piece> pSecondPiece;
    shared_ptr<Piece> pUndoPiece;
    const auto [file, rank] = m->getTarget();
    const auto [prevFile, prevRank] = m->getInit();
    int capturedFile = -1, capturedRank = -1;
    int secondFileInit = -1, secondFileTarget = -1;

    const int castleRank = (m->getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0;
    arrowList_ = m->getMoveArrows();
    shared_ptr<Piece> selected = m->getSelectedPiece();

    // TODO smooth transition for castle
    switch (m->getMoveType())
    {
        case MoveType::NORMAL:
            m_board.resetBoardTile(file, rank);
            m_board.setBoardTile(prevFile, prevRank, selected);
            if (dynamic_cast<King*>(selected.get()))
            {
                selected->setAsFirstMovement();
            }

            break;
        case MoveType::CAPTURE:
            pUndoPiece = pCaptured;
            capturedFile = file;
            capturedRank = rank;
            m_board.setBoardTile(file, rank, pCaptured);
            m_board.setBoardTile(prevFile, prevRank, selected);
            break;
        case MoveType::ENPASSANT:
            pUndoPiece = pCaptured;

            assert(m->getEnPassantCapturedPieceInitialPos().has_value());
            capturedFile = m->getEnPassantCapturedPieceInitialPos().value().first;
            capturedRank = m->getEnPassantCapturedPieceInitialPos().value().second;

            m_board.resetBoardTile(file, rank);
            m_board.setBoardTile(capturedFile, capturedRank, pCaptured);
            m_board.setBoardTile(prevFile, prevRank, selected);
            break;
        case MoveType::CASTLE_KINGSIDE:
            secondFileInit = 5;
            secondFileTarget = 7;
            pSecondPiece = pCaptured;
            m_board.resetBoardTile(secondFileInit, castleRank);
            m_board.resetBoardTile(6, castleRank);
            m_board.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            m_board.setBoardTile(prevFile, prevRank, selected);

            selected->setAsFirstMovement();
            pSecondPiece->setAsFirstMovement();
            break;
        case MoveType::CASTLE_QUEENSIDE:
            secondFileInit = 3;
            secondFileTarget = 0;
            pSecondPiece = pCaptured;
            m_board.resetBoardTile(secondFileInit, castleRank);
            m_board.resetBoardTile(2, castleRank);
            m_board.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            m_board.setBoardTile(prevFile, prevRank, selected);

            selected->setAsFirstMovement();
            pSecondPiece->setAsFirstMovement();
            break;
        case MoveType::INIT_SPECIAL:
            m_board.resetBoardTile(file, rank);
            m_board.setBoardTile(prevFile, prevRank, selected);
            selected->setAsFirstMovement();
            break;
        case MoveType::NEWPIECE:
            shared_ptr<Piece> pawn = make_shared<Pawn>(m->getSelectedPiece()->getTeam(), prevRank, prevFile);
            m_board.setBoardTile(file, rank, pCaptured);
            m_board.setBoardTile(prevFile, prevRank, pawn);
    }

    if (enableTransition_)
    {
        // Enable transition movement
        setTransitioningPiece(
            true, selected, file, rank, prevFile, prevRank, pUndoPiece,
                capturedFile, capturedRank
        );

        if (pSecondPiece) {
            setSecondTransitioningPiece(
                pSecondPiece, secondFileInit, castleRank,
                secondFileTarget, castleRank
            );
        }
    }

    --m_moveIterator;
}

void MoveTreeManager::setTransitioningPiece(
    bool isUndo_, shared_ptr<Piece>& p_, int initialFile_, int initialRank_,
    int targetFile_, int targetRank_, shared_ptr<Piece>& captured_,
    int capturedFile_, int capturedRank_
)
{
    m_transitioningPiece.resetPieces();
    m_transitioningPiece.setTransitioningPiece(p_);
    m_transitioningPiece.setDestination({ui::getWindowXPos(targetFile_), ui::getWindowYPos(targetRank_)});
    m_transitioningPiece.setCurrPos({ui::getWindowXPos(initialFile_), ui::getWindowYPos(initialRank_)});
    m_transitioningPiece.setCapturedPiece(captured_, ui::getWindowXPos(capturedFile_), ui::getWindowYPos(capturedRank_));
    m_transitioningPiece.setUndo(isUndo_);
    m_transitioningPiece.setIsTransitioning(true);
    m_transitioningPiece.setIncrement();
}

void MoveTreeManager::setSecondTransitioningPiece(
    shared_ptr<Piece>& p_, int initialFile_, int initialRank_,
    int targetFile_, int targetRank_
)
{
    m_transitioningPiece.setSecondTransitioningPiece(p_);
    m_transitioningPiece.setSecondDestination({ui::getWindowXPos(targetFile_), ui::getWindowYPos(targetRank_)});
    m_transitioningPiece.setSecondCurrPos({ui::getWindowXPos(initialFile_), ui::getWindowYPos(initialRank_)});
    m_transitioningPiece.setSecondIsTransitioning(true);
    m_transitioningPiece.setSecondIncrement();
}
