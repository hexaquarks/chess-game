#include "../../include/Utilities/MoveTreeManager.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/GameThread.hpp"

MoveTreeManager::MoveTreeManager(Board& board_): game(board_)
{
}

bool MoveTreeManager::goToPreviousMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    if (!m_moveIterator.isAtTheBeginning())
    {
        undoMove(enableTransition_, arrowList_);
        game.switchTurn();
        game.updateAllCurrentlyAvailableMoves();
        
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
        game.switchTurn();
        game.updateAllCurrentlyAvailableMoves();

        return true;
    }
    return false;
}

void MoveTreeManager::addMove(shared_ptr<Move>& move_, vector<Arrow>& arrowList_)
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
    shared_ptr<Move>& move_, 
    bool addToList_, 
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    if (!move_) return;
    
    const int castleRow = (game.getTurn() == Team::WHITE)? 7: 0;
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
    game.resetBoardTile(prevFile, prevRank);
    arrowList_ = move_->getMoveArrows();

    switch (move_->getMoveType())
    {
        case MoveType::NORMAL:
            game.setBoardTile(file, rank, pSelectedPiece);
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
            auto pOldPiece = game.getBoardTile(file, rank);
            game.setBoardTile(file, rank, pSelectedPiece);
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
            game.resetBoardTile(oldCoors.first, oldCoors.second);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pCapturedPiece, oldCoors), m_moveIterator);
            }
            break;

        case MoveType::CASTLE_KINGSIDE:
        {
            secondFileInit = 7;
            secondFileTarget = 5;
            pSecondPieceOpt = game.getBoardTile(secondFileInit, castleRow);
            game.resetBoardTile(secondFileInit, castleRow);
            game.setBoardTile(secondFileTarget, castleRow, pSecondPieceOpt.value());
            game.setBoardTile(6, castleRow, pSelectedPiece);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {6, castleRow};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPieceOpt.value()), m_moveIterator);
            }
            break;
        }

        case MoveType::CASTLE_QUEENSIDE:
        {
            secondFileInit = 0;
            secondFileTarget = 3;
            auto pSecondPiece = game.getBoardTile(secondFileInit, castleRow);
            game.resetBoardTile(secondFileInit, castleRow);
            game.setBoardTile(secondFileTarget, castleRow, pSecondPiece);
            game.setBoardTile(2, castleRow, pSelectedPiece);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {2, castleRow};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPiece), m_moveIterator);
            }
            break;
        }

        case MoveType::INIT_SPECIAL:
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            break;

        case MoveType::NEWPIECE:
        {
            auto pOldPiece = game.getBoardTile(file, rank);
            pPromotingPieceOpt = make_shared<Queen>(pSelectedPiece->getTeam(), rank, file);
            Piece::setLastMovedPiece(pPromotingPieceOpt.value());
            game.setBoardTile(file, rank, pPromotingPieceOpt.value());
            game.addPiece(pPromotingPieceOpt.value());
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
                    pSecondPieceOpt.value(), secondFileInit, castleRow,
                    secondFileTarget, castleRow
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
                false, pSecondPieceOpt.value(), secondFileInit, castleRow, secondFileTarget, castleRow,
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

    const int castleRow = (m->getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0;
    arrowList_ = m->getMoveArrows();
    // TODO smooth transition for castle
    switch (m->getMoveType())
    {
        case MoveType::NORMAL:
            game.resetBoardTile(file, rank);
            break;
        case MoveType::CAPTURE:
            pUndoPiece = pCaptured;
            capturedFile = file;
            capturedRank = rank;
            game.setBoardTile(file, rank, pCaptured);
            break;
        case MoveType::ENPASSANT:
            pUndoPiece = pCaptured;
            capturedFile = m->getSpecial().first;
            capturedRank = m->getSpecial().second;
            game.resetBoardTile(file, rank);
            game.setBoardTile(capturedFile, capturedRank, pCaptured);
            break;
        case MoveType::CASTLE_KINGSIDE:
            secondFileInit = 5;
            secondFileTarget = 7;
            pSecondPiece = pCaptured;
            game.setKingAsFirstMovement();
            game.resetBoardTile(secondFileInit, castleRow);
            game.resetBoardTile(6, castleRow);
            game.setBoardTile(secondFileTarget, castleRow, pSecondPiece);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            secondFileInit = 3;
            secondFileTarget = 0;
            pSecondPiece = pCaptured;
            game.setKingAsFirstMovement();
            game.resetBoardTile(secondFileInit, castleRow);
            game.resetBoardTile(2, castleRow);
            game.setBoardTile(secondFileTarget, castleRow, pSecondPiece);
            break;
        case MoveType::INIT_SPECIAL:
            game.resetBoardTile(file, rank);
            break;
        case MoveType::NEWPIECE:
            shared_ptr<Piece> pawn = make_shared<Pawn>(m->getSelectedPiece()->getTeam(), prevRank, prevFile);
            game.setBoardTile(file, rank, pCaptured);
            game.setBoardTile(prevFile, prevRank, pawn);
    }

    shared_ptr<Piece> selected = m->getSelectedPiece();
    game.setBoardTile(prevFile, prevRank, selected);

    if (enableTransition_)
    {
        // Enable transition movement
        setTransitioningPiece(
            true, selected, file, rank, prevFile, prevRank, pUndoPiece,
                capturedFile, capturedRank
        );

        if (pSecondPiece) {
            setSecondTransitioningPiece(
                pSecondPiece, secondFileInit, castleRow,
                secondFileTarget, castleRow
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

