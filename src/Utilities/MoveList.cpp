#include "../../include/Utilities/MoveList.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/GameThread.hpp"

MoveList::MoveList(Board& board_): game(board_)
{
}

bool MoveList::goToPreviousMove(bool enableTransition_, vector<Arrow>& arrowList_)
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

bool MoveList::goToNextMove(
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

void MoveList::addMove(shared_ptr<Move>& move_, vector<Arrow>& arrowList_)
{
    applyMove(move_, true, true, arrowList_);
}

void MoveList::applyMove(
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    applyMove(m_moveIterator->m_move, false, enableTransition_, arrowList_);
}

void MoveList::applyMove(shared_ptr<Move>& move_, bool addToList_, bool enableTransition_, vector<Arrow>& arrowList_)
{
    if (!move_) return;
    
    const int castleRow = (game.getTurn() == Team::WHITE)? 7: 0;
    shared_ptr<Piece> pOldPiece;
    shared_ptr<Piece> pSecondPiece;
    shared_ptr<Piece> pPromotingPiece;
    shared_ptr<Piece> pSelectedPiece = move_->getSelectedPiece();
    shared_ptr<Piece> pCapturedPiece;
    int capturedX = -1, capturedY = -1;

    coor2d oldCoors;
    const auto [prevX, prevY] = move_->getInit();
    const auto [x, y] = move_->getTarget();
    int secondXInit = -1, secondXTarget = -1;

    // Set the current tile of the piece null. Necessary for navigating back to current move through goToNextMove()
    game.resetBoardTile(prevX, prevY);
    arrowList_ = move_->getMoveArrows();
    // TODO smooth piece transition for castle
    switch (move_->getMoveType())
    {
        case MoveType::NORMAL:
            game.setBoardTile(x, y, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            // soundMove.play();
            break;

        case MoveType::CAPTURE:
            pCapturedPiece = move_->getCapturedPiece();
            capturedX = x;
            capturedY = y;
            pOldPiece = game.getBoardTile(x, y);
            game.setBoardTile(x, y, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pOldPiece), m_moveIterator);
            }
            // soundCapture.play();
            break;

        case MoveType::ENPASSANT:
            pCapturedPiece = move_->getCapturedPiece();
            capturedX = pCapturedPiece->getY();
            capturedY = pCapturedPiece->getX();
            oldCoors = {capturedX, capturedY};
            game.resetBoardTile(oldCoors.first, oldCoors.second);
            game.setBoardTile(x, y, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pCapturedPiece, oldCoors), m_moveIterator);
            }
            break;

        case MoveType::CASTLE_KINGSIDE:
            secondXInit = 7;
            secondXTarget = 5;
            pSecondPiece = game.getBoardTile(secondXInit, castleRow);
            game.resetBoardTile(secondXInit, castleRow);
            game.setBoardTile(secondXTarget, castleRow, pSecondPiece);
            game.setBoardTile(6, castleRow, pSelectedPiece);
            game.setBoardTile(x, y, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {6, castleRow};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPiece), m_moveIterator);
            }
            break;

        case MoveType::CASTLE_QUEENSIDE:
            secondXInit = 0;
            secondXTarget = 3;
            pSecondPiece = game.getBoardTile(secondXInit, castleRow);
            game.resetBoardTile(secondXInit, castleRow);
            game.setBoardTile(secondXTarget, castleRow, pSecondPiece);
            game.setBoardTile(2, castleRow, pSelectedPiece);
            game.setBoardTile(x, y, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {2, castleRow};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPiece), m_moveIterator);
            }
            break;

        case MoveType::INIT_SPECIAL:
            game.setBoardTile(x, y, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            break;

        case MoveType::NEWPIECE:
            pOldPiece = game.getBoardTile(x, y);
            pPromotingPiece = make_shared<Queen>(pSelectedPiece->getTeam(), y, x);
            Piece::setLastMovedPiece(pPromotingPiece);
            game.setBoardTile(x, y, pPromotingPiece);
            game.addPiece(pPromotingPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pOldPiece), m_moveIterator);
            }
            break;
    }

    if (enableTransition_)
    {
        if (!addToList_ && pSelectedPiece)
        {
            // Enable piece visual transition
            setTransitioningPiece(
                false, pSelectedPiece, prevX, prevY, x, y, pCapturedPiece,
                capturedX, capturedY
            );

            if (pSecondPiece) {
                setSecondTransitioningPiece(
                    pSecondPiece, secondXInit, castleRow,
                    secondXTarget, castleRow
                );
            }

            if (pPromotingPiece) {
                getTransitioningPiece().setPromotingPiece(pPromotingPiece);
            }
        }
        else if (pSecondPiece)
        {
            // Enable rook sliding when user just castled
            setTransitioningPiece(
                false, pSecondPiece, secondXInit, castleRow, secondXTarget, castleRow,
                pCapturedPiece, capturedX, capturedY
            );
        }
    }
}

void MoveList::undoMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    shared_ptr<Move> m = m_moveIterator->m_move;
    if (!m) return;

    shared_ptr<Piece> pCaptured = m->getCapturedPiece();
    shared_ptr<Piece> pSecondPiece;
    shared_ptr<Piece> pUndoPiece;
    const auto [x, y] = m->getTarget();
    const auto [prevX, prevY] = m->getInit();
    int capturedX = -1, capturedY = -1;
    int secondXInit = -1, secondXTarget = -1;

    const int castleRow = (m->getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0;
    arrowList_ = m->getMoveArrows();
    // TODO smooth transition for castle
    switch (m->getMoveType())
    {
        case MoveType::NORMAL:
            game.resetBoardTile(x, y);
            break;
        case MoveType::CAPTURE:
            pUndoPiece = pCaptured;
            capturedX = x;
            capturedY = y;
            game.setBoardTile(x, y, pCaptured);
            break;
        case MoveType::ENPASSANT:
            pUndoPiece = pCaptured;
            capturedX = m->getSpecial().first;
            capturedY = m->getSpecial().second;
            game.resetBoardTile(x, y);
            game.setBoardTile(capturedX, capturedY, pCaptured);
            break;
        case MoveType::CASTLE_KINGSIDE:
            secondXInit = 5;
            secondXTarget = 7;
            pSecondPiece = pCaptured;
            game.setKingAsFirstMovement();
            game.resetBoardTile(secondXInit, castleRow);
            game.resetBoardTile(6, castleRow);
            game.setBoardTile(secondXTarget, castleRow, pSecondPiece);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            secondXInit = 3;
            secondXTarget = 0;
            pSecondPiece = pCaptured;
            game.setKingAsFirstMovement();
            game.resetBoardTile(secondXInit, castleRow);
            game.resetBoardTile(2, castleRow);
            game.setBoardTile(secondXTarget, castleRow, pSecondPiece);
            break;
        case MoveType::INIT_SPECIAL:
            game.resetBoardTile(x, y);
            break;
        case MoveType::NEWPIECE:
            shared_ptr<Piece> pawn = make_shared<Pawn>(m->getSelectedPiece()->getTeam(), prevY, prevX);
            game.setBoardTile(x, y, pCaptured);
            game.setBoardTile(prevX, prevY, pawn);
    }

    shared_ptr<Piece> selected = m->getSelectedPiece();
    game.setBoardTile(prevX, prevY, selected);

    if (enableTransition_)
    {
        // Enable transition movement
        setTransitioningPiece(
            true, selected, x, y, prevX, prevY, pUndoPiece,
                capturedX, capturedY
        );

        if (pSecondPiece) {
            setSecondTransitioningPiece(
                pSecondPiece, secondXInit, castleRow,
                secondXTarget, castleRow
            );
        }
    }

    --m_moveIterator;
}

void MoveList::setTransitioningPiece(
    bool isUndo_, shared_ptr<Piece>& p_, int initialX_, int initialY_,
    int xTarget_, int yTarget_, shared_ptr<Piece>& captured_,
    int capturedXPos_, int capturedYPos_
)
{
    m_transitioningPiece.resetPieces();
    m_transitioningPiece.setTransitioningPiece(p_);
    m_transitioningPiece.setDestination({ui::getWindowXPos(xTarget_), ui::getWindowYPos(yTarget_)});
    m_transitioningPiece.setCurrPos({ui::getWindowXPos(initialX_), ui::getWindowYPos(initialY_)});
    m_transitioningPiece.setCapturedPiece(captured_, ui::getWindowXPos(capturedXPos_), ui::getWindowYPos(capturedYPos_));
    m_transitioningPiece.setUndo(isUndo_);
    m_transitioningPiece.setIsTransitioning(true);
    m_transitioningPiece.setIncrement();
}

void MoveList::setSecondTransitioningPiece(
    shared_ptr<Piece>& p_, int initialX_, int initialY_,
    int xTarget_, int yTarget_
)
{
    m_transitioningPiece.setSecondTransitioningPiece(p_);
    m_transitioningPiece.setSecondDestination({ui::getWindowXPos(xTarget_), ui::getWindowYPos(yTarget_)});
    m_transitioningPiece.setSecondCurrPos({ui::getWindowXPos(initialX_), ui::getWindowYPos(initialY_)});
    m_transitioningPiece.setSecondIsTransitioning(true);
    m_transitioningPiece.setSecondIncrement();
}

