#include "../include/MoveList.hpp"

MoveList::MoveList(Board& board): game(board) {}

void MoveList::highlightLastMove(RenderWindow& window) const {
    Move& move = *moveIterator;
    if (move.getCapturedPiece() == nullptr) return;

    RectangleShape squareBefore(Vector2f(CELL_SIZE, CELL_SIZE));
    RectangleShape squareAfter(Vector2f(CELL_SIZE, CELL_SIZE));

    Color color = ((move.getXInit() + move.getYInit()) % 2 == 0)
        ? Color(205, 210, 106) : Color(170, 162, 58);
    squareBefore.setFillColor(color);
    squareAfter.setFillColor(color);

    squareBefore.setPosition(move.getXInit()*CELL_SIZE, move.getYInit()*CELL_SIZE);
    squareAfter.setPosition(move.getXTarget()*CELL_SIZE, move.getYTarget()*CELL_SIZE);

    window.draw(squareBefore);
    window.draw(squareAfter);
}

void MoveList::goToPreviousMove() {
    if (hasMovesBefore()) {
        undoMove();
        ++moveIterator; // Go to previous move
    }
}

void MoveList::goToNextMove() { 
    if (hasMovesAfter()) {
        --moveIterator; // Go to previous move
        applyMove();
    }
}

void MoveList::addMove( MoveType moveType, int x, int y, int prevX, int prevY,
    Piece* selectedPiece, Piece* lastMove) {
    applyMove(moveType, x, y, prevX, prevY, selectedPiece, lastMove);
    moveIterator = moves.begin();
}

void MoveList::applyMove(MoveType moveType, int x, int y, int prevX, int prevY,
    Piece* selectedPiece, Piece* lastMove) {
    const int castleRow = (game.getTurn() == Team::WHITE)? 7: 0;
    Piece* oldPiece = nullptr;

    // Set the current tile of the piece null. Necessary for navigating back to current move through goToNextMove()
    game.setBoardTile(prevX, prevY, nullptr); 

    switch (moveType) {
        case MoveType::NORMAL:
            game.setBoardTile(x, y, selectedPiece);
            moves.emplace_front(Move(x, y, prevX, prevY, selectedPiece, MoveType::NORMAL));
            // soundMove.play();
            break;
        case MoveType::CAPTURE:
            oldPiece = game.getBoardTile(x, y);
            game.setBoardTile(x, y, selectedPiece);
            moves.emplace_front(Move(x, y, prevX, prevY, selectedPiece, oldPiece, MoveType::CAPTURE));
            // soundCapture.play();
            break;
        case MoveType::ENPASSANT:
            oldPiece = game.getBoardTile(lastMove->getY(),lastMove->getX()); // the position of the captured pawn
            game.setBoardTile(x, y, selectedPiece);
            game.setBoardTile(lastMove->getY(), lastMove->getX(), nullptr);
            moves.emplace_front(Move(x, y, prevX, prevY, 
                selectedPiece, oldPiece, MoveType::ENPASSANT));
            break;
        case MoveType::CASTLE_KINGSIDE:
            oldPiece = game.getBoardTile(7, castleRow);
            game.setBoardTile(5, castleRow, game.getBoardTile(7, castleRow));
            game.setBoardTile(7, castleRow, nullptr);
            game.setBoardTile(6, castleRow, selectedPiece);
            moves.emplace_front(Move(6, castleRow, prevX, prevY, 
                selectedPiece, oldPiece, MoveType::CASTLE_KINGSIDE));
            break;
        case MoveType::CASTLE_QUEENSIDE:
            oldPiece = game.getBoardTile(7, castleRow);
            game.setBoardTile(3, castleRow, game.getBoardTile(0, castleRow));
            game.setBoardTile(0, castleRow, nullptr);
            game.setBoardTile(2, castleRow, selectedPiece);
            moves.emplace_front(Move(2, castleRow, prevX, prevY, 
                selectedPiece, oldPiece, MoveType::CASTLE_QUEENSIDE));
            break;
        case MoveType::INIT_SPECIAL:
            game.setBoardTile(x, y, selectedPiece);
            moves.emplace_front(Move(x, y,prevX, prevY, 
                selectedPiece, MoveType::INIT_SPECIAL));
            break;
        case MoveType::NEWPIECE:
            selectedPiece->move(-1, -1); // Deleted
            Queen* queen = new Queen(game.getTurn(), y, x);
            game.setBoardTile(x, y, queen);
            game.addPiece(queen);
            break;
    }
}

void MoveList::applyMove() {
    Move& m = *moveIterator;
    applyMove(m.getMoveType(), m.getXTarget(),
         m.getYTarget(), m.getXInit(), m.getYInit(), 
         m.getSelectedPiece(), m.getCapturedPiece());
}

void MoveList::undoMove() {
    Move& m = *moveIterator;
    game.setBoardTile(m.getXInit(), m.getYInit(), m.getSelectedPiece()); // Set the moved piece back
    int castleRow = (m.getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0; 

    switch (m.getMoveType()) {
        case MoveType::NORMAL:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), nullptr); 
            break;
        case MoveType::CAPTURE:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), m.getCapturedPiece()); 
            break;
        case MoveType::ENPASSANT:
            game.setBoardTile(m.getXTarget(), m.getYTarget(), nullptr);            
            game.setBoardTile(m.getCapturedPiece()->getY(), 
                m.getCapturedPiece()->getX(), m.getCapturedPiece());
            break;
        case MoveType::CASTLE_KINGSIDE:
            game.setBoardTile(7, castleRow, m.getCapturedPiece());
            game.setBoardTile(6, castleRow, nullptr);
            game.setBoardTile(5, castleRow, nullptr);
            break;
        case MoveType::CASTLE_QUEENSIDE:
            game.setBoardTile(0, castleRow, m.getCapturedPiece());
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
