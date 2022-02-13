#include "../include/MoveList.hpp"
#include "../include/PieceTransition.hpp"
#include "../include/GameThread.hpp"

MoveList::MoveList(Board& board): game(board) {}

void MoveList::highlightLastMove(RenderWindow& window) const {
    Move& move = *m_moveIterator;

    RectangleShape squareBefore = createSquare();
    RectangleShape squareAfter = createSquare();

    Color colorInit = ((move.getXInit()+move.getYInit()) % 2)? Color(170, 162, 58): Color(205, 210, 106);
    Color colorTarget = ((move.getXTarget()+move.getYTarget()) % 2)? Color(170, 162, 58): Color(205, 210, 106);
    squareBefore.setFillColor(colorInit);
    squareAfter.setFillColor(colorTarget);

    squareBefore.setPosition(GameThread::getWindowXPos(move.getXInit()),
        GameThread::getWindowYPos(move.getYInit()));
    squareAfter.setPosition(GameThread::getWindowXPos(move.getXTarget()), 
        GameThread::getWindowYPos(move.getYTarget()));

    window.draw(squareBefore);
    window.draw(squareAfter);
}

void MoveList::goToPreviousMove(PieceTransition& piece) {
    if (hasMovesBefore()) {
        cout << "has moves before" << endl;
        undoMove(piece);
        ++m_moveIterator; // Go to previous move
    }
}

void MoveList::goToNextMove() { 
    if (hasMovesAfter()) {
        cout << "has moves after" << endl;
        --m_moveIterator; // Go to previous move
        applyMove();
    }
}

void MoveList::addMove(MoveType moveType, int x, int y, int prevX, int prevY,
    Piece* selectedPiece, Piece* lastMove) {
    applyMove(moveType, x, y, prevX, prevY, selectedPiece, lastMove, true);
    m_moveIterator = m_moves.begin();
}

void MoveList::applyMove(MoveType moveType, int x, int y, int prevX, int prevY,
    Piece* p, Piece* lastMove, bool addToList) {
    const int castleRow = (game.getTurn() == Team::WHITE)? 7: 0;
    Piece* oldPiece = nullptr;

    // Set the current tile of the piece null. Necessary for navigating back to current move through goToNextMove()
    game.setBoardTile(prevX, prevY, nullptr); 

    switch (moveType) {
        case MoveType::NORMAL:
            game.setBoardTile(x, y, p);
            if (addToList) m_moves.emplace_front(Move(x, y, prevX, prevY, p, MoveType::NORMAL));
            // soundMove.play();
            break;

        case MoveType::CAPTURE:
            oldPiece = game.getBoardTile(x, y);
            game.setBoardTile(x, y, p);
            if (addToList) m_moves.emplace_front(Move(x, y, prevX, prevY, p, oldPiece, MoveType::CAPTURE));
            // soundCapture.play();
            break;

        case MoveType::ENPASSANT:
            oldPiece = game.getBoardTile(lastMove->getY(), lastMove->getX()); // The position of the captured pawn
            game.setBoardTile(x, y, p);
            game.setBoardTile(lastMove->getY(), lastMove->getX(), nullptr);
            if (addToList) m_moves.emplace_front(Move(x, y, prevX, prevY, p, oldPiece, MoveType::ENPASSANT));
            break;

        case MoveType::CASTLE_KINGSIDE:
            oldPiece = game.getBoardTile(7, castleRow);
            game.setBoardTile(5, castleRow, game.getBoardTile(7, castleRow));
            game.setBoardTile(7, castleRow, nullptr);
            game.setBoardTile(6, castleRow, p);
            if (addToList) m_moves.emplace_front(Move(6, castleRow, prevX, prevY, p, oldPiece, MoveType::CASTLE_KINGSIDE));
            break;

        case MoveType::CASTLE_QUEENSIDE:
            oldPiece = game.getBoardTile(7, castleRow);
            game.setBoardTile(3, castleRow, game.getBoardTile(0, castleRow));
            game.setBoardTile(0, castleRow, nullptr);
            game.setBoardTile(2, castleRow, p);
            if (addToList) m_moves.emplace_front(Move(2, castleRow, prevX, prevY, p, oldPiece, MoveType::CASTLE_QUEENSIDE));
            break;

        case MoveType::INIT_SPECIAL:
            game.setBoardTile(x, y, p);
            if (addToList) m_moves.emplace_front(Move(x, y, prevX, prevY, p, MoveType::INIT_SPECIAL));
            break;

        case MoveType::NEWPIECE:
            p->move(-1, -1); // Deleted
            Queen* queen = new Queen(game.getTurn(), y, x);
            game.setBoardTile(x, y, queen);
            game.addPiece(queen);
            break;
    }
}

void MoveList::applyMove() {
    Move& m = *m_moveIterator;
    applyMove(
        m.getMoveType(), m.getXTarget(),
        m.getYTarget(), m.getXInit(), m.getYInit(), 
        m.getSelectedPiece(), m.getCapturedPiece(), false
    );
}

void MoveList::undoMove(PieceTransition& piece) {
    Move& m = *m_moveIterator;
    Piece* captured = m.getCapturedPiece();
    // game.setBoardTile(m.getXInit(), m.getYInit(), m.getSelectedPiece()); // Set the moved piece back

    GameThread::setTransitioningPiece(m.getSelectedPiece(),
        m.getXInit() * CELL_SIZE, m.getYInit() * CELL_SIZE, piece); 
        
    int castleRow = (m.getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0;

    switch (m.getMoveType()) {
        case MoveType::NORMAL:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), nullptr);
            break;
        case MoveType::CAPTURE:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), captured);
            break;
        case MoveType::ENPASSANT:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), nullptr);
            game.setBoardTile(captured->getY(), captured->getX(), captured);
            break;
        case MoveType::CASTLE_KINGSIDE:
            game.setBoardTile(7, castleRow, captured);
            game.setBoardTile(6, castleRow, nullptr);
            game.setBoardTile(5, castleRow, nullptr);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            game.setBoardTile(0, castleRow, captured);
            game.setBoardTile(2, castleRow, nullptr);
            game.setBoardTile(3, castleRow, nullptr);
            break;
        case MoveType::INIT_SPECIAL:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), nullptr);
            break;
        case MoveType::NEWPIECE:
            // TODO
            break;
    }
}
