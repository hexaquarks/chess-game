#include "ChessGame.h"
#include <iostream>

ChessGame::ChessGame() {

    // defining the pieces 

    pw1 = new Pawn(Piece::WHITE, std::pair<int,int>(0,1));
    pw2 = new Pawn(Piece::WHITE, std::pair<int,int>(1,1));
    pw3 = new Pawn(Piece::WHITE, std::pair<int,int>(2,1));
    pw4 = new Pawn(Piece::WHITE, std::pair<int,int>(3,1));
    pw5 = new Pawn(Piece::WHITE, std::pair<int,int>(4,1));
    pw6 = new Pawn(Piece::WHITE, std::pair<int,int>(5,1));
    pw7 = new Pawn(Piece::WHITE, std::pair<int,int>(6,1));
    pw8 = new Pawn(Piece::WHITE, std::pair<int,int>(7,1));
    
    pb1 = new Pawn(Piece::BLACK, std::pair<int,int>(0,6));
    pb2 = new Pawn(Piece::BLACK, std::pair<int,int>(1,6));
    pb4 = new Pawn(Piece::BLACK, std::pair<int,int>(2,6));
    pb3 = new Pawn(Piece::BLACK, std::pair<int,int>(3,6));
    pb5 = new Pawn(Piece::BLACK, std::pair<int,int>(4,6));
    pb6 = new Pawn(Piece::BLACK, std::pair<int,int>(5,6));
    pb7 = new Pawn(Piece::BLACK, std::pair<int,int>(6,6));
    pb8 = new Pawn(Piece::BLACK, std::pair<int,int>(7,6));

    rw1 = new Rook(Piece::WHITE, std::pair<int,int>(0,1));
    rw2 = new Rook(Piece::WHITE, std::pair<int,int>(0,1));
    rb1 = new Rook(Piece::WHITE, std::pair<int,int>(0,1));
    rb2 = new Rook(Piece::WHITE, std::pair<int,int>(0,1));

    nw1 = new Knight(Piece::WHITE, std::pair<int,int>(0,1));
    nw2 = new Knight(Piece::WHITE, std::pair<int,int>(0,1));
    nb1 = new Knight(Piece::WHITE, std::pair<int,int>(0,1));
    nb2 = new Knight(Piece::WHITE, std::pair<int,int>(0,1));

    bb1 = new Bishop(Piece::WHITE, std::pair<int,int>(0,1));
    bb2 = new Bishop(Piece::WHITE, std::pair<int,int>(0,1));
    bw1 = new Bishop(Piece::WHITE, std::pair<int,int>(0,1));
    bw2 = new Bishop(Piece::WHITE, std::pair<int,int>(0,1));

    kb1 = new King(Piece::WHITE, std::pair<int,int>(0,1));
    kw1 = new King(Piece::WHITE, std::pair<int,int>(0,1));
    qb1 = new Queen(Piece::WHITE, std::pair<int,int>(0,1));
    qw1 = new Queen(Piece::WHITE, std::pair<int,int>(0,1));


    // defining the board

    // black pawns
    _board[1][1] = pb2;
    _board[0][1] = pb1;
    _board[2][1] = pb3;
    _board[3][1] = pb4;
    _board[4][1] = pb5;
    _board[5][1] = pb6;
    _board[6][1] = pb7;
    _board[7][1] = pb8; 

    // black minors, king and queen
    _board[1][0] = rb1;
    _board[7][0] = rb2;
    _board[2][0] = nb1;
    _board[6][0] = nb2;
    _board[3][0] = bb1;
    _board[5][0] = bb2;
    _board[4][0] = qb1;
    _board[5][0] = kb1;

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
    _board[1][7] = rw1;
    _board[7][7] = rw2;
    _board[2][7] = nw1;
    _board[6][7] = nw2;
    _board[3][7] = bw1;
    _board[5][7] = bw2;
    _board[4][7] = kw1;
    _board[5][7] = qw1;

    // remaining board squares are set to NULL

    for(int i = 2 ; i < 6 ; i++) {
        for (int j = 0 ; j < 8 ; j++) {
            _board[j][i] = nullptr;
        }
    }
}

Piece* ChessGame::getBoardPiece(int row, int col) {
    return _board[row][col];
}