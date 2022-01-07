#include "ChessGame.h"
#include <iostream>

ChessGame::ChessGame() {

    // defining the pieces 

    pw1 = new Pawn(Piece::WHITE, std::pair<int,int>(0,6));
    pw2 = new Pawn(Piece::WHITE, std::pair<int,int>(1,6));
    pw3 = new Pawn(Piece::WHITE, std::pair<int,int>(2,6));
    pw4 = new Pawn(Piece::WHITE, std::pair<int,int>(3,6));
    pw5 = new Pawn(Piece::WHITE, std::pair<int,int>(4,6));
    pw6 = new Pawn(Piece::WHITE, std::pair<int,int>(5,6));
    pw7 = new Pawn(Piece::WHITE, std::pair<int,int>(6,6));
    pw8 = new Pawn(Piece::WHITE, std::pair<int,int>(7,6));
    
    pb1 = new Pawn(Piece::BLACK, std::pair<int,int>(0,1));
    pb2 = new Pawn(Piece::BLACK, std::pair<int,int>(1,1));
    pb4 = new Pawn(Piece::BLACK, std::pair<int,int>(2,1));
    pb3 = new Pawn(Piece::BLACK, std::pair<int,int>(3,1));
    pb5 = new Pawn(Piece::BLACK, std::pair<int,int>(4,1));
    pb6 = new Pawn(Piece::BLACK, std::pair<int,int>(5,1));
    pb7 = new Pawn(Piece::BLACK, std::pair<int,int>(6,1));
    pb8 = new Pawn(Piece::BLACK, std::pair<int,int>(7,1));

    rw1 = new Rook(Piece::WHITE, std::pair<int,int>(0,7));
    rw2 = new Rook(Piece::WHITE, std::pair<int,int>(7,7));
    rb1 = new Rook(Piece::BLACK, std::pair<int,int>(0,0));
    rb2 = new Rook(Piece::BLACK, std::pair<int,int>(7,0));

    nw1 = new Knight(Piece::WHITE, std::pair<int,int>(1,7));
    nw2 = new Knight(Piece::WHITE, std::pair<int,int>(6,7));
    nb1 = new Knight(Piece::BLACK, std::pair<int,int>(1,0));
    nb2 = new Knight(Piece::BLACK, std::pair<int,int>(6,0));

    bb1 = new Bishop(Piece::BLACK, std::pair<int,int>(2,0));
    bb2 = new Bishop(Piece::BLACK, std::pair<int,int>(5,0));
    bw1 = new Bishop(Piece::WHITE, std::pair<int,int>(2,7));
    bw2 = new Bishop(Piece::WHITE, std::pair<int,int>(5,7));

    kb1 = new King(Piece::BLACK, std::pair<int,int>(3,0));
    kw1 = new King(Piece::WHITE, std::pair<int,int>(3,0));
    qb1 = new Queen(Piece::BLACK, std::pair<int,int>(4,7));
    qw1 = new Queen(Piece::WHITE, std::pair<int,int>(4,7));


    // defining the board

    // black pawns
    _board[0][1] = pb1;
    _board[1][1] = pb2;
    _board[2][1] = pb3;
    _board[3][1] = pb4;
    _board[4][1] = pb5;
    _board[5][1] = pb6;
    _board[6][1] = pb7;
    _board[7][1] = pb8; 

    // black minors, king and queen
    _board[0][0] = rb1;
    _board[7][0] = rb2;
    _board[1][0] = nb1;
    _board[6][0] = nb2;
    _board[2][0] = bb1;
    _board[5][0] = bb2;
    _board[3][0] = qb1;
    _board[4][0] = kb1;

    // white pawns
    _board[0][6] = pw1;
    _board[1][6] = pw2;
    _board[2][6] = pw3;
    _board[3][6] = pw4;
    _board[4][6] = pw5;
    _board[5][6] = pw6;
    _board[6][6] = pw7;
    _board[7][6] = pw8;

    // white minors, king and queen
    _board[0][7] = rw1;
    _board[7][7] = rw2;
    _board[1][7] = nw1;
    _board[6][7] = nw2;
    _board[2][7] = bw1;
    _board[5][7] = bw2;
    _board[3][7] = kw1;
    _board[4][7] = qw1;

    // remaining board squares are set to NULL

    for(int i = 2 ; i < 6 ; i++) {
        for (int j = 0 ; j < 8 ; j++) {
            _board[j][i] = nullptr;
        }
    }
}

ChessGame::~ChessGame() {}

Piece* ChessGame::getBoardTile(int row, int col) {
    return _board[row][col];
}