#include "../include/ChessGame.hpp"
#include <algorithm>

void ChessGame::freeMemory() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            m_board[i][j] = nullptr;
    for (Piece* piece: m_whitePieces) delete piece;
    for (Piece* piece: m_blackPieces) delete piece;
    m_whitePieces.clear();
    m_blackPieces.clear();
}

void ChessGame::reset() {
    freeMemory(); // Free up memory

    // Set the kings
    m_blackKing = new King(Team::BLACK, 0, 4);
    m_whiteKing = new King(Team::WHITE, 7, 4);

    // Set first row (black)
    m_board[0][0] = new Rook(Team::BLACK, 0, 0);
    m_board[0][1] = new Knight(Team::BLACK, 0, 1);
    m_board[0][2] = new Bishop(Team::BLACK, 0, 2);
    m_board[0][3] = new Queen(Team::BLACK, 0, 3);
    m_board[0][4] = m_blackKing;
    m_board[0][5] = new Bishop(Team::BLACK, 0, 5);
    m_board[0][6] = new Knight(Team::BLACK, 0, 6);
    m_board[0][7] = new Rook(Team::BLACK, 0, 7);

    // Set last row (white)
    m_board[7][0] = new Rook(Team::WHITE, 7, 0);
    m_board[7][1] = new Knight(Team::WHITE, 7, 1);
    m_board[7][2] = new Bishop(Team::WHITE, 7, 2);
    m_board[7][3] = new Queen(Team::WHITE, 7, 3);
    m_board[7][4] = m_whiteKing;
    m_board[7][5] = new Bishop(Team::WHITE, 7, 5);
    m_board[7][6] = new Knight(Team::WHITE, 7, 6);
    m_board[7][7] = new Rook(Team::WHITE, 7, 7);

    // Fill in white and black pawns
    for (int col = 0; col < 8; ++col) {
        m_board[1][col] = new Pawn(Team::BLACK, 1, col); // Black pawn on row 1
        m_board[6][col] = new Pawn(Team::WHITE, 6, col); // White pawn on row 6
    }

    // Add every black piece to the list of black pieces
    for (int row = 0; row < 2; ++row)
        for (int col = 0; col < 8; ++col)
            m_blackPieces.push_back(m_board[row][col]);
    
    // Add every white piece to the list of white pieces
    for (int row = 6; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            m_whitePieces.push_back(m_board[row][col]);
}

void ChessGame::setBoardTile(int x, int y, Piece* piece, bool record = true) {
    // Set up the piece
    m_board[y][x] = piece;
    if (piece != nullptr) piece->move(y, x, record); 
}

void ChessGame::applyMove(moveType* selectedMove, int x, int y,int prevX, int prevY, Piece* selectedPiece, Piece* lastMove, int CELL_SIZE, list<Move>& moveSequence) {
    const int castleRow = (getTurn() == Team::WHITE)? 7: 0;    
    x /= CELL_SIZE;
    y /= CELL_SIZE; 
    Piece* oldPiece = nullptr;

    switch (get<1>(*selectedMove)) {
        case MoveType::NORMAL:
            setBoardTile(x, y, selectedPiece);

            moveSequence.emplace_front(Move(x, y, prevX, prevY, selectedPiece, MoveType::NORMAL));
            // soundMove.play();
            break;
        case MoveType::CAPTURE:
            oldPiece = getBoardTile(x, y); // possible bug here ? 
            setBoardTile(x, y, selectedPiece);

            moveSequence.emplace_front(Move(x, y,prevX, prevY, selectedPiece, oldPiece, MoveType::CAPTURE));
            // soundCapture.play();
            break;
        case MoveType::ENPASSANT:
            oldPiece = getBoardTile(lastMove->getY(),lastMove->getX()); // the position of the captured pawn
            setBoardTile(x, y, selectedPiece);
            setBoardTile(lastMove->getY(), lastMove->getX(), nullptr);

            moveSequence.emplace_front(Move(x, y,prevX, prevY, selectedPiece, oldPiece, MoveType::ENPASSANT));
            break;
        case MoveType::CASTLE_KINGSIDE:
            oldPiece = getBoardTile(7, castleRow);
            setBoardTile(5, castleRow, getBoardTile(7, castleRow));
            setBoardTile(7, castleRow, nullptr);
            setBoardTile(6, castleRow, selectedPiece);

            moveSequence.emplace_front(Move(6, castleRow,prevX, prevY,  selectedPiece, oldPiece, MoveType::CASTLE_KINGSIDE));
            break;
        case MoveType::CASTLE_QUEENSIDE:
            oldPiece = getBoardTile(7, castleRow);
            setBoardTile(3, castleRow, getBoardTile(0, castleRow));
            setBoardTile(0, castleRow, nullptr);
            setBoardTile(2, castleRow, selectedPiece);

            moveSequence.emplace_front(Move(2, castleRow,prevX, prevY,  selectedPiece, oldPiece, MoveType::CASTLE_QUEENSIDE));
            break;
        case MoveType::INIT_SPECIAL:
            setBoardTile(x, y, selectedPiece);

            moveSequence.emplace_front(Move(x, y,prevX, prevY, selectedPiece, MoveType::INIT_SPECIAL));
            cout << "pushed coordinates init are -> " << "(m_xInit, m_yInit) : " <<selectedPiece->getX()<<","<<selectedPiece->getY()<<endl;
            cout << "pushed coordinates target are -> " << "(m_xTarget, m_yTarget) : " <<x<<","<<y<<endl;
            break;
        case MoveType::NEWPIECE:
            selectedPiece->move(-1, -1); // Deleted
            Queen* queen = new Queen(getTurn(), y, x);
            setBoardTile(x, y, queen);
            addPiece(queen);

            // moveSequence TODO
            break;
    }
}

void ChessGame::undoMove(list<Move>::iterator& it) {
    
    setBoardTile((*it).m_xInit, (*it).m_yInit, (*it).getSelectedPiece()); // set the moved piece back
    int castleRow = ((*it).getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0; 

    switch((*it).getMoveType()){
        case MoveType::NORMAL:
            setBoardTile((*it).m_xTarget, (*it).m_yTarget, nullptr); 
            break;
        case MoveType::CAPTURE:
            setBoardTile((*it).m_xTarget, (*it).m_yTarget, (*it).getCapturedPiece()); 
            break;
        case MoveType::ENPASSANT:
            setBoardTile((*it).m_xTarget, (*it).m_yTarget, nullptr);            
            setBoardTile((*it).getCapturedPiece()->getY(), (*it).getCapturedPiece()->getX(), (*it).getCapturedPiece());
            break;
        case MoveType::CASTLE_KINGSIDE:
            setBoardTile(7, castleRow, (*it).getCapturedPiece());
            break;
        case MoveType::CASTLE_QUEENSIDE:
            setBoardTile(0, castleRow, (*it).getCapturedPiece());
            break;
        case MoveType::INIT_SPECIAL:
            cout << "in the init special" <<endl;
            cout << "coordinates init are -> " << "(m_xInit, m_yInit) : " <<(*it).m_xInit<<","<<(*it).m_yInit<<endl;
            cout << "coordinates target are -> " << "(m_xTarget, m_yTarget) : " <<(*it).m_xTarget<<","<<(*it).m_yTarget<<endl;
            setBoardTile((*it).m_xTarget, (*it).m_yTarget, nullptr); 
            break;
        case MoveType::NEWPIECE:
            // TODO
            break;
    }
}
