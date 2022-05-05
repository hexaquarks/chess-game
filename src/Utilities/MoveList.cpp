#include "../../include/Utilities/MoveList.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/GameThread.hpp"

MoveList::MoveList(Board& board_, PieceTransition& p_)
: game(board_), m_transitioningPiece(p_) {}

void MoveList::highlightLastMove(RenderWindow& window_) const {
    if (m_moves.empty()) return;
    Move& move = *m_moveIterator;

    RectangleShape squareBefore = createSquare();
    RectangleShape squareAfter = createSquare();

    Color colorInit = ((move.getInit().first+move.getInit().second) % 2)
                    ? Color(170, 162, 58): Color(205, 210, 106);
    Color colorTarget = ((move.getTarget().first+move.getTarget().second) % 2)
                    ? Color(170, 162, 58): Color(205, 210, 106);
    squareBefore.setFillColor(colorInit);
    squareAfter.setFillColor(colorTarget);

    squareBefore.setPosition(
        GameThread::getWindowXPos(GameThread::boardFlipped()? 7-move.getInit().first: move.getInit().first),
        GameThread::getWindowYPos(GameThread::boardFlipped()? 7-move.getInit().second: move.getInit().second)
    );
    squareAfter.setPosition(
        GameThread::getWindowXPos(GameThread::boardFlipped()? 7-move.getTarget().first: move.getTarget().first), 
        GameThread::getWindowYPos(GameThread::boardFlipped()? 7-move.getTarget().second: move.getTarget().second)
    );

    window_.draw(squareBefore);
    window_.draw(squareAfter);
}

void MoveList::goToPreviousMove(bool enableTransition_, vector<Arrow>& arrowList_) {
    if (hasMovesBefore()) {
        undoMove(enableTransition_, arrowList_);
        ++m_moveIterator; // Go to previous move
        game.switchTurn();
    }
}

void MoveList::goToNextMove(bool enableTransition_, vector<Arrow>& arrowList_) { 
    if (hasMovesAfter()) {
        --m_moveIterator; // Go to previous move
        applyMove(enableTransition_, arrowList_);
        game.switchTurn();
    }
}

void MoveList::addMove(Move& move_, vector<Arrow>& arrowList_) {
    applyMove(move_, true, true, arrowList_);
    m_moveIterator = m_moves.begin();
}

void MoveList::applyMove(bool enableTransition_, vector<Arrow>& arrowList_) {
    Move& m = *m_moveIterator;
    applyMove(m, false, enableTransition_, arrowList_);
}

void MoveList::applyMove(Move& move_, bool addToList_, bool enableTransition_, vector<Arrow>& arrowList_) {
    const int castleRow = (game.getTurn() == Team::WHITE)? 7: 0;
    Piece* pOldPiece = nullptr;
    Piece* pSelectedPiece = move_.getSelectedPiece();
    Piece* pCapturedPiece = move_.getCapturedPiece();

    coor2d oldCoors;
    int prevX = move_.getInit().first;
    int prevY = move_.getInit().second;
    int x = move_.getTarget().first;
    int y = move_.getTarget().second;

    // Set the current tile of the piece null. Necessary for navigating back to current move through goToNextMove()
    game.setBoardTile(prevX, prevY, nullptr); 
    arrowList_ = move_.getMoveArrows();
    // TODO smooth piece transition for castle 
    switch (move_.getMoveType()) {
        case MoveType::NORMAL:
            if (addToList_) {
                m_moves.emplace_front(move_);
                game.setBoardTile(x, y, pSelectedPiece);
            }
            // soundMove.play();
            break;

        case MoveType::CAPTURE:
            pOldPiece = game.getBoardTile(x, y);
            if (addToList_) {
                game.setBoardTile(x, y, pSelectedPiece);
                m_moves.emplace_front(Move(move_, pOldPiece));
            }
            // soundCapture.play();
            break;

        case MoveType::ENPASSANT:
            oldCoors = { pCapturedPiece->getY(), pCapturedPiece->getX() };
            game.setBoardTile(oldCoors.first, oldCoors.second, nullptr);
            if (addToList_) {
                game.setBoardTile(x, y, pSelectedPiece);
                m_moves.emplace_front(Move(move_, pCapturedPiece, oldCoors));
            }
            break;

        case MoveType::CASTLE_KINGSIDE:
            pOldPiece = game.getBoardTile(7, castleRow);
            game.setBoardTile(7, castleRow, nullptr);
            game.setBoardTile(5, castleRow, pOldPiece);
            game.setBoardTile(6, castleRow, pSelectedPiece);
            if (addToList_) {
                coor2d target = make_pair(6, castleRow);
                move_.setTarget(target);
                m_moves.emplace_front(Move(move_, pOldPiece));
            }
            break;

        case MoveType::CASTLE_QUEENSIDE:
            pOldPiece = game.getBoardTile(0, castleRow);
            game.setBoardTile(0, castleRow, nullptr);
            game.setBoardTile(3, castleRow, pOldPiece);
            game.setBoardTile(2, castleRow, pSelectedPiece);
            if (addToList_) {
                coor2d target = make_pair(2, castleRow);
                move_.setTarget(target);
                m_moves.emplace_front(Move(move_, pOldPiece));
            }
            break;

        case MoveType::INIT_SPECIAL:
            if (addToList_) {
                game.setBoardTile(x, y, pSelectedPiece);
                m_moves.emplace_front(move_);
            }
            break;

        case MoveType::NEWPIECE:
            // Possible leaking memory here actually ? 
            pOldPiece = game.getBoardTile(x, y);
            Queen* queen = new Queen(pSelectedPiece->getTeam(), y, x);
            Piece::setLastMovedPiece(queen);
            game.setBoardTile(x, y, queen);
            if (addToList_) {
                m_moves.emplace_front(Move(move_, pOldPiece));
            }
            break;
    }

    if (!addToList_ && pSelectedPiece != nullptr) {
        if (enableTransition_) {
            // move the piece from the (-1, -1) hidden location back to the square 
            // where it begins it's transition
            pSelectedPiece->move(prevY, prevX);

            // enable piece visual transition
            GameThread::setTransitioningPiece(pSelectedPiece,
                x*g_CELL_SIZE, y*g_CELL_SIZE, getTransitioningPiece()
            ); 
        } else {
            game.setBoardTile(x, y, pSelectedPiece);
        }
    }
}

void MoveList::undoMove(bool enableTransition_, vector<Arrow>& arrowList_) {
    Move& m = *m_moveIterator;
    Piece* pCaptured = m.getCapturedPiece();
    int x = m.getTarget().first;
    int y = m.getTarget().second;
    int prevX = m.getInit().first;
    int prevY = m.getInit().second;

    int castleRow = (m.getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0;
    arrowList_ = m.getMoveArrows();
    // TODO smooth transition for castle 
    switch (m.getMoveType()) {
        case MoveType::NORMAL:
            game.setBoardTile(x, y, nullptr);
            break;
        case MoveType::CAPTURE:
            game.setBoardTile(x, y, pCaptured);
            break;
        case MoveType::ENPASSANT:
            game.setBoardTile(x, y, nullptr);
            game.setBoardTile(m.getSpecial().first, m.getSpecial().second, pCaptured);
            break;
        case MoveType::CASTLE_KINGSIDE:
            game.setBoardTile(5, castleRow, nullptr);
            game.setBoardTile(6, castleRow, nullptr);
            game.setBoardTile(7, castleRow, pCaptured);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            game.setBoardTile(3, castleRow, nullptr);
            game.setBoardTile(2, castleRow, nullptr);
            game.setBoardTile(0, castleRow, pCaptured);
            break;
        case MoveType::INIT_SPECIAL:
            game.setBoardTile(x, y, nullptr);
            break;
        case MoveType::NEWPIECE:
            Piece* queen = game.getBoardTile(x, y);
            game.setBoardTile(x, y, pCaptured);
            delete queen;
    }

    if (enableTransition_) {
        // move the piece from the (-1,-1) hidden location back to the square 
        // where it begins it's transition
        m.getSelectedPiece()->move(y, x); 

        // Enable transition movement
        GameThread::setTransitioningPiece(
            m.getSelectedPiece(), m.getInit().first * g_CELL_SIZE, 
            m.getInit().second * g_CELL_SIZE, getTransitioningPiece()
        );
    } else {
        game.setBoardTile(m.getInit().first, m.getInit().second, m.getSelectedPiece());
    }
}
