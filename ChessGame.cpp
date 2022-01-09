#include "ChessGame.h"
#include <algorithm>

void ChessGame::freeMemory() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board[i][j] = nullptr;
    for (Piece* piece: whitePieces) delete piece;
    for (Piece* piece: blackPieces) delete piece;
    whitePieces.clear();
    blackPieces.clear();
}

void ChessGame::reset() {
    freeMemory(); // Free up memory

    // Set the kings
    blackKing = new King(Team::BLACK, 0, 4);
    whiteKing = new King(Team::WHITE, 7, 4);

    // Set first row (black)
    setBoardTile(0, 0, new Rook(Team::BLACK, 0, 0));
    setBoardTile(0, 1, new Knight(Team::BLACK, 0, 1));
    setBoardTile(0, 2, new Bishop(Team::BLACK, 0, 2));
    setBoardTile(0, 3, new Queen(Team::BLACK, 0, 3));
    setBoardTile(0, 4, blackKing);
    setBoardTile(0, 5, new Bishop(Team::BLACK, 0, 5));
    setBoardTile(0, 6, new Knight(Team::BLACK, 0, 6));
    setBoardTile(0, 7, new Rook(Team::BLACK, 0, 7));

    // Set last row (white)
    setBoardTile(7, 0, new Rook(Team::WHITE, 7, 0));
    setBoardTile(7, 1, new Knight(Team::WHITE, 7, 1));
    setBoardTile(7, 2, new Bishop(Team::WHITE, 7, 2));
    setBoardTile(7, 3, new Queen(Team::WHITE, 7, 3));
    setBoardTile(7, 4, whiteKing);
    setBoardTile(7, 5, new Bishop(Team::WHITE, 7, 5));
    setBoardTile(7, 6, new Knight(Team::WHITE, 7, 6));
    setBoardTile(7, 7, new Rook(Team::WHITE, 7, 7));

    // Fill in white and black pawns
    for (int col = 0; col < 8; ++col) {
        setBoardTile(1, col, new Pawn(Team::BLACK, 1, col)); // Black pawn on row 1
        setBoardTile(6, col, new Pawn(Team::WHITE, 6, col)); // White pawn on row 6
    }

    // Add every black piece to the list of black pieces
    for (int row = 0; row < 2; ++row)
        for (int col = 0; col < 8; ++col)
            blackPieces.push_back(getBoardTile(row, col));
    
    // Add every white piece to the list of white pieces
    for (int row = 6; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            whitePieces.push_back(getBoardTile(row, col));
}

void ChessGame::setBoardTile(int x, int y, Piece* piece) {
    Piece* initial = board[x][y];

    /*// If a piece was already in this position, remove it
    if (initial != nullptr) {
        if (initial->getTeam() == Team::BLACK) removeFrom(blackPieces, initial);
        else removeFrom(whitePieces, initial);
        delete initial;
    }*/

    // Set up the piece
    board[x][y] = piece;
    if (piece != nullptr) piece->move(x, y); 
}
