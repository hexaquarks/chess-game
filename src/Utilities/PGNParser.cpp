#include "../../include/Utilities/PGNParser.hpp"
#include "../../include/Utilities/MoveTreeManager.hpp"
#include "../../include/Utilities/Move.hpp"

#include <fstream>
#include <cassert> 
#include <iterator>
#include <algorithm>

namespace 
{
    std::pair<char, int> getTargetFileAndRank(
        const std::string& move_, 
        bool isCapture_, 
        bool isCheckOrCheckMate_) 
    {
        size_t targetFileIndex = 0;
        size_t targetRankIndex = 0;

        if (isCapture_) {
            targetFileIndex = move_.find('x') + 1;
            targetRankIndex = move_.find('x') + 2;
        } else if (isCheckOrCheckMate_) {
            targetFileIndex = move_.size() - 3;
            targetRankIndex = move_.size() - 2;
        } else {
            targetFileIndex = move_.size() - 2;
            targetRankIndex = move_.size() - 1;
        }

        return std::make_pair(move_[targetFileIndex], move_[targetRankIndex] - '0');
    }

    bool pngIsPossibleMove(
        const Move& possibleMove_, 
        PieceType pieceType_, 
        Team currTeamTurn_,
        bool isCapture_)
    {
        const auto pMovePiece = possibleMove_.getSelectedPiece();
        // Piece should not be nullptr. Every move has a piece assocaited
        assert(pMovePiece); 

        return (pMovePiece->getType() == pieceType_ && 
                pMovePiece->getTeam() == currTeamTurn_ );
    }

    PieceType pieceTypeletterToEnum(char pngPieceTypeLetter_)
    {
         switch (pngPieceTypeLetter_) 
         {
            case 'N': return PieceType::KNIGHT;
            case 'B': return PieceType::BISHOP;
            case 'R': return PieceType::ROOK;
            case 'Q': return PieceType::QUEEN;
            case 'K': return PieceType::KING;
            case 'p': return PieceType::PAWN;
            default: {
                std::cout << "actual letter is " << pngPieceTypeLetter_<< std::endl;
                assert(false); // Handle new piece type.
            }
        }

        return PieceType::PAWN;
    }
}

void addMove(std::vector<std::string>& movesString, const std::string& lastMove) {
    if (
        !lastMove.empty() && lastMove != "1/2-1/2" &&
        lastMove != "*" && lastMove != "1-0" &&
        lastMove != "0-1" && lastMove != "0-0" &&
        lastMove != "0-1/2" && lastMove != "1/2-0"
    ) {
        movesString.push_back(lastMove);
    }
}

PGNParser::PGNParser(MoveTreeManager& moveTreeManager_)
: m_moveTreeManager(moveTreeManager_)
{
}

void PGNParser::loadFromFile(const char* fileName) {
    moves.clear();
    std::string line;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return;
    }

    while (getline(file, line)) {
        bool ignore = false;
        std::string lastMove;
        for (char c: line) {
            if (c == '}' || c == ']') {
                ignore = false;
                continue;
            }

            if (ignore) {
                continue;
            }

            switch (c) {
                case ' ':
                    addMove(moves, lastMove);
                    lastMove.clear();
                    break;
                case '.':
                    lastMove.clear();
                    break;
                case '{': case '[':
                    addMove(moves, lastMove);
                    ignore = true;
                    break;
                default:
                    lastMove += c;
            }
        }
        addMove(moves, lastMove);
    }

    file.close();
}

void PGNParser::loadFromFile(const std::string& fileName) {
    loadFromFile(fileName.c_str());
}


std::vector<std::string> PGNParser::tokenizePGN(const std::string& pgn_) 
{
    if (pgn_.size() == 0) return {};
    std::string processedPgn = pgn_;
   
    // Remove move numbers with three dots
    for (size_t i = 3; i < processedPgn.size(); ++i) 
    {
        if (processedPgn[i] == '.' && 
            processedPgn[i - 1] == '.' &&
            processedPgn[i - 2] == '.' &&
            std::isdigit(processedPgn[i - 3])) 
        {
            int k = 0;
            if (i - 4 > 0 && std::isdigit(processedPgn[i - 4])) ++k;
            if (i - 5 > 0 && std::isdigit(processedPgn[i - 5])) ++k;
            processedPgn.erase(i - 3 - k, 4 + k); 
            i -= (3 + k); 
        }
    }

    // Remove move numbers with one dot
    for (size_t i = 1; i < processedPgn.size(); ++i) 
    {
        if (processedPgn[i] == '.' && std::isdigit(processedPgn[i - 1])) 
        {
             int k = 0;
            if (i - 2 > 0 && std::isdigit(processedPgn[i - 2])) ++k;
            if (i - 3 > 0 && std::isdigit(processedPgn[i - 3])) ++k;
            processedPgn.erase(i - 1 - k, 2 + k); 
            i -= (1 + k); 

        }
    }

    // Add space before each right parenthesis
    for (size_t i = 0; i < processedPgn.size(); ++i) 
    {
        if (processedPgn[i] == ')') 
        {
            processedPgn.insert(i, " ");
            ++i; 
        }
    }

    // Split by spaces
    std::vector<std::string> tokens;
    std::istringstream ss(processedPgn);
    std::string token;
    while (ss >> token) tokens.push_back(token);

    return tokens;
}

void PGNParser::parseAllTokens(
    const std::vector<std::string>& tokens_, 
    size_t& index_, 
    int& moveCount_, 
    std::stack<int>& undoStack_) 
{
    while (index_ < tokens_.size()) 
    {
        std::vector<Arrow> dummyArrows{};

        const std::string& token = tokens_[index_++];

        if (token == "(") 
        {
            m_moveTreeManager.goToPreviousMove(false, dummyArrows);
            undoStack_.push(moveCount_);

            // Reset the move count for the variation
            moveCount_ = 0;
            continue;
        }

        if (token == ")") 
        {
            // Get the number of moves to undo
            int undoCount = undoStack_.top();
            undoStack_.pop();

            // Undo the moves in the current variation
            for (size_t i = 0; i < moveCount_; ++i) 
            {
                std::vector<Arrow> dummyArrows{};
                m_moveTreeManager.goToPreviousMove(false, dummyArrows);
            }
            m_moveTreeManager.goToNextMove(false, 0, dummyArrows);
            moveCount_ = undoCount;
            continue;
        }

        addMoveToPGNTree(token);
        ++moveCount_;
        //getMoves().printTree();
    }
}


void PGNParser::addMoveToPGNTree(const std::string& token_)
{   
    std::string moveToken = token_;

    std::vector<Move> allPossibleMoves = m_moveTreeManager.getBoard().getAllCurrentlyAvailableMoves();
    std::vector<Move> actualPossibleMoves;

    const bool isCastle = moveToken == "O-O" || moveToken == "O-O-O";
    if (isCastle) {
        auto castleMoveFilterFunc = [&moveToken](const Move& move_)
        {
            return (moveToken == "O-O" && move_.getMoveType() == MoveType::CASTLE_KINGSIDE) ||
                   (moveToken == "O-O-O" && move_.getMoveType() == MoveType::CASTLE_QUEENSIDE);
        };
        std::copy_if(
            allPossibleMoves.begin(), 
            allPossibleMoves.end(), 
            std::back_inserter(actualPossibleMoves),
            castleMoveFilterFunc);

        // There should be exactly one matching castle move in all posssible 
        // moves, given a castling move token.
        assert(actualPossibleMoves.size() == 1);
    } else {
        // Normal move, i.e., anything besides castling

        // Flags for special conditions
        const bool isCapture = moveToken.find('x') != std::string::npos;
        const bool isCheck = moveToken.find('+') != std::string::npos;
        const bool isCheckMate = moveToken.find('#') != std::string::npos;

        auto [pgnTargetFile, pgnTargetRank] = getTargetFileAndRank(moveToken, isCapture, isCheck || isCheckMate);

        // Determine piece type letter from PNG
        const bool isPawnMove = moveToken.size() == 2 || std::islower(moveToken[0]);
        char pngPieceTypeLetter = isPawnMove ? 'p' : moveToken[0];

        // Determine the piece and source square
        PieceType pngPieceType = pieceTypeletterToEnum(pngPieceTypeLetter);

        Team currTeamTurn = m_moveTreeManager.getBoard().getTurn();
        auto normalMoveFilterFunc = 
            [&pngPieceType, &currTeamTurn, &isCapture, &pgnTargetFile = pgnTargetFile, &pgnTargetRank = pgnTargetRank]
            (const Move& move_)
            {
                const auto [moveTargetFile, moveTargetRank] =  move_.getTarget();
                    // Transform to array-based coordinates
                int pgnTargetFileInt = pgnTargetFile - 'a';
                int pgnTargetRankInt = 8 - pgnTargetRank;

                const bool preValidation = pngIsPossibleMove(move_, pngPieceType, currTeamTurn, isCapture);
                const bool moveTargetSquareMatch = moveTargetFile == pgnTargetFileInt && moveTargetRank == pgnTargetRankInt;

                return preValidation && moveTargetSquareMatch;
            };
        std::copy_if(
            allPossibleMoves.begin(), 
            allPossibleMoves.end(), 
            std::back_inserter(actualPossibleMoves),
            normalMoveFilterFunc);
    }

    // Every png token should match with at least one move from all currently available moves
    assert(actualPossibleMoves.size() != 0);

    // TODO treat edge cases for multiple moves available, for instance
    // There could be 2 knights that can capture the same piece.

    Move selectedMove = actualPossibleMoves[0];
    const auto [moveInitialFile, moveInitialRank] =  selectedMove.getInit();
    const auto [moveTargetFile, moveTargetRank] =  selectedMove.getTarget();
    
    auto pMove = m_moveTreeManager.getBoard().applyMoveOnBoardTesting(
        selectedMove.getMoveType(),
        std::make_pair(moveTargetFile, moveTargetRank),
        std::make_pair(moveInitialFile, moveInitialRank),
        selectedMove.getSelectedPiece());

    std::vector<Arrow> arrows;
    m_moveTreeManager.addMove(pMove, arrows); 
    m_moveTreeManager.getBoard().updateBoardInfosAfterNewMove(selectedMove.getSelectedPiece(), pMove);
}

void PGNParser::initializeMoveSequenceFromPNG(const std::string& pgn_)
{
    m_moveTreeManager.getBoard().updateAllCurrentlyAvailableMoves();
    // Tokenize the PGN string 
    std::vector<std::string> tokens = tokenizePGN(pgn_);
    if (tokens.size() == 0) return;

    size_t index = 0;
    int moveCount = 0;
    std::stack<int> undoStack;
    parseAllTokens(tokens, index, moveCount, undoStack);
}