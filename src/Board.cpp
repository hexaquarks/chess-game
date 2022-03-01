#include "../include/Board.hpp"
#include "../include/King.hpp"
#include <algorithm>

void Board::freeMemory() {
    for (uint8_t i = 0; i < 8; ++i)
        for (uint8_t j = 0; j < 8; ++j)
            m_board[i][j] = nullptr;
    for (Piece* piece: m_whitePieces) delete piece;
    for (Piece* piece: m_blackPieces) delete piece;
    m_whitePieces.clear();
    m_blackPieces.clear();
}

void Board::reset() {
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
    for (uint8_t col = 0; col < 8; ++col) {
        m_board[1][col] = new Pawn(Team::BLACK, 1, col); // Black pawn on row 1
        m_board[6][col] = new Pawn(Team::WHITE, 6, col); // White pawn on row 6
    }

    // Add every black piece to the list of black pieces
    for (uint8_t row = 0; row < 2; ++row)
        for (uint8_t col = 0; col < 8; ++col)
            m_blackPieces.push_back(m_board[row][col]);
    
    // Add every white piece to the list of white pieces
    for (uint8_t row = 6; row < 8; ++row)
        for (uint8_t col = 0; col < 8; ++col)
            m_whitePieces.push_back(m_board[row][col]);

    m_turn = Team::WHITE; // reset the first move to be for white
}

void Board::setBoardTile(int x, int y, Piece* piece, bool record) {
    // Set up the piece
    m_board[y][x] = piece;
    if (piece != nullptr) piece->move(y, x, record); 
}

vector<Move> Board::calculateAllMoves() {
    vector<Move> moves;
    vector<Piece*> playerPieces = (m_turn == Team::WHITE)? m_whitePieces: m_blackPieces;
    for (Piece* piece: playerPieces) {
        vector<Move> pieceMoves = possibleMovesFor(piece);
        removeIllegalMoves(pieceMoves, piece);
        for (auto& move: pieceMoves) {
            moves.push_back(move);
        }
    }
    return moves;
}

void Board::removeIllegalMoves(vector<Move>& possibleMoves, Piece* selectedPiece) {
    vector<Move>::iterator it = possibleMoves.begin();

    while (it != possibleMoves.end()) {
        int x = (*it).getTarget().second;
        int y = (*it).getTarget().first;

        // Store piece occupied by target square
        Piece* temp = getBoardTile(x, y);

        int initialX = selectedPiece->getY();
        int initialY = selectedPiece->getX();

        setBoardTile(x, y, selectedPiece, false); // Move this piece to target square
        setBoardTile(initialX, initialY, nullptr, false); // Set null to selected piece's square

        if (kingIsChecked()) it = possibleMoves.erase(it);
        else ++it;

        setBoardTile(initialX, initialY, selectedPiece, false);
        setBoardTile(x, y, temp, false); 
    }
}
