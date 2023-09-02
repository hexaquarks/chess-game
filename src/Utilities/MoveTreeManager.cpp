#include "../../include/Utilities/MoveTreeManager.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/GameThread.hpp"

#include <cassert> 
#include <algorithm>
#include <iterator>

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

MoveTreeManager::MoveTreeManager(Board& board_): game(board_)
{
}

bool MoveTreeManager::goToPreviousMove(bool enableTransition_, vector<Arrow>& arrowList_)
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

bool MoveTreeManager::goToNextMove(
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

void MoveTreeManager::addMove(const shared_ptr<Move>& move_, vector<Arrow>& arrowList_)
{
    applyMove(move_, true, true, arrowList_);
}

void MoveTreeManager::applyMove(
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    applyMove(m_moveIterator->m_move, false, enableTransition_, arrowList_);
}

void MoveTreeManager::applyMove(
    const shared_ptr<Move>& move_, 
    bool addToList_, 
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    if (!move_) return;
    
    const int castleRank = (game.getTurn() == Team::WHITE)? 7: 0;
    std::optional<shared_ptr<Piece>> pSecondPieceOpt;
    std::optional<shared_ptr<Piece>> pPromotingPieceOpt;
    shared_ptr<Piece> pSelectedPiece = move_->getSelectedPiece();
    shared_ptr<Piece> pCapturedPiece; // TODO change to optional. Requires some refactoring...
    int capturedFile = -1, capturedRank = -1;

    coor2d oldCoors;
    const auto [prevFile, prevRank] = move_->getInit();
    const auto [file, rank] = move_->getTarget();
    int secondFileInit = -1, secondFileTarget = -1;

    // Set the current tile of the piece null. Necessary for navigating back to current move through goToNextMove()
    game.resetBoardTile(prevFile, prevRank);
    arrowList_ = move_->getMoveArrows();

    switch (move_->getMoveType())
    {
        case MoveType::NORMAL:
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            // soundMove.play();
            break;

        case MoveType::CAPTURE:
        {
            pCapturedPiece = move_->getCapturedPiece();
            capturedFile = file;
            capturedRank = rank;
            auto pOldPiece = game.getBoardTile(file, rank);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pOldPiece), m_moveIterator);
            }
            // soundCapture.play();
            break;
        }

        case MoveType::ENPASSANT:
            pCapturedPiece = move_->getCapturedPiece();
            capturedFile = pCapturedPiece->getFile();
            capturedRank = pCapturedPiece->getRank();
            oldCoors = {capturedFile, capturedRank};
            game.resetBoardTile(oldCoors.first, oldCoors.second);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pCapturedPiece, oldCoors), m_moveIterator);
            }
            break;

        case MoveType::CASTLE_KINGSIDE:
        {
            secondFileInit = 7;
            secondFileTarget = 5;
            pSecondPieceOpt = game.getBoardTile(secondFileInit, castleRank);
            game.resetBoardTile(secondFileInit, castleRank);
            game.setBoardTile(secondFileTarget, castleRank, pSecondPieceOpt.value());
            game.setBoardTile(6, castleRank, pSelectedPiece);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {6, castleRank};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPieceOpt.value()), m_moveIterator);
            }
            break;
        }

        case MoveType::CASTLE_QUEENSIDE:
        {
            secondFileInit = 0;
            secondFileTarget = 3;
            auto pSecondPiece = game.getBoardTile(secondFileInit, castleRank);
            game.resetBoardTile(secondFileInit, castleRank);
            game.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            game.setBoardTile(2, castleRank, pSelectedPiece);
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {2, castleRank};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPiece), m_moveIterator);
            }
            break;
        }

        case MoveType::INIT_SPECIAL:
            game.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            break;

        case MoveType::NEWPIECE:
        {
            auto pOldPiece = game.getBoardTile(file, rank);
            pPromotingPieceOpt = make_shared<Queen>(pSelectedPiece->getTeam(), rank, file);
            Piece::setLastMovedPiece(pPromotingPieceOpt.value());
            game.setBoardTile(file, rank, pPromotingPieceOpt.value());
            game.addPiece(pPromotingPieceOpt.value());
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pOldPiece), m_moveIterator);
            }
            break;
        }
    }

    if (enableTransition_)
    {
        if (!addToList_ && pSelectedPiece)
        {
            // Enable piece visual transition
            setTransitioningPiece(
                false, pSelectedPiece, prevFile, prevRank, file, rank, pCapturedPiece,
                capturedFile, capturedRank
            );

            if (pSecondPieceOpt.has_value()) {
                setSecondTransitioningPiece(
                    pSecondPieceOpt.value(), secondFileInit, castleRank,
                    secondFileTarget, castleRank
                );
            }

            if (pPromotingPieceOpt.has_value()) {
                getTransitioningPiece().setPromotingPiece(pPromotingPieceOpt.value());
            }
        }
        else if (pSecondPieceOpt.has_value())
        {
            // Enable rook sliding when user just castled
            setTransitioningPiece(
                false, pSecondPieceOpt.value(), secondFileInit, castleRank, secondFileTarget, castleRank,
                pCapturedPiece, capturedFile, capturedRank
            );
        }
    }
}

void MoveTreeManager::undoMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    shared_ptr<Move> m = m_moveIterator->m_move;
    if (!m) return;

    shared_ptr<Piece> pCaptured = m->getCapturedPiece();
    shared_ptr<Piece> pSecondPiece;
    shared_ptr<Piece> pUndoPiece;
    const auto [file, rank] = m->getTarget();
    const auto [prevFile, prevRank] = m->getInit();
    int capturedFile = -1, capturedRank = -1;
    int secondFileInit = -1, secondFileTarget = -1;

    const int castleRank = (m->getSelectedPiece()->getTeam() == Team::WHITE)? 7: 0;
    arrowList_ = m->getMoveArrows();
    shared_ptr<Piece> selected = m->getSelectedPiece();

    // TODO smooth transition for castle
    switch (m->getMoveType())
    {
        case MoveType::NORMAL:
            game.resetBoardTile(file, rank);
            game.setBoardTile(prevFile, prevRank, selected);
            if (dynamic_cast<King*>(selected.get()))
            {
                selected->setAsFirstMovement();
            }

            break;
        case MoveType::CAPTURE:
            pUndoPiece = pCaptured;
            capturedFile = file;
            capturedRank = rank;
            game.setBoardTile(file, rank, pCaptured);
            game.setBoardTile(prevFile, prevRank, selected);
            break;
        case MoveType::ENPASSANT:
            pUndoPiece = pCaptured;
            capturedFile = m->getSpecial().first;
            capturedRank = m->getSpecial().second;
            game.resetBoardTile(file, rank);
            game.setBoardTile(capturedFile, capturedRank, pCaptured);
            game.setBoardTile(prevFile, prevRank, selected);
            break;
        case MoveType::CASTLE_KINGSIDE:
            secondFileInit = 5;
            secondFileTarget = 7;
            pSecondPiece = pCaptured;
            game.resetBoardTile(secondFileInit, castleRank);
            game.resetBoardTile(6, castleRank);
            game.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            game.setBoardTile(prevFile, prevRank, selected);

            selected->setAsFirstMovement();
            pSecondPiece->setAsFirstMovement();
            break;
        case MoveType::CASTLE_QUEENSIDE:
            secondFileInit = 3;
            secondFileTarget = 0;
            pSecondPiece = pCaptured;
            game.resetBoardTile(secondFileInit, castleRank);
            game.resetBoardTile(2, castleRank);
            game.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            game.setBoardTile(prevFile, prevRank, selected);

            selected->setAsFirstMovement();
            pSecondPiece->setAsFirstMovement();
            break;
        case MoveType::INIT_SPECIAL:
            game.resetBoardTile(file, rank);
            game.setBoardTile(prevFile, prevRank, selected);
            selected->setAsFirstMovement();
            break;
        case MoveType::NEWPIECE:
            shared_ptr<Piece> pawn = make_shared<Pawn>(m->getSelectedPiece()->getTeam(), prevRank, prevFile);
            game.setBoardTile(file, rank, pCaptured);
            game.setBoardTile(prevFile, prevRank, pawn);
    }

    if (enableTransition_)
    {
        // Enable transition movement
        setTransitioningPiece(
            true, selected, file, rank, prevFile, prevRank, pUndoPiece,
                capturedFile, capturedRank
        );

        if (pSecondPiece) {
            setSecondTransitioningPiece(
                pSecondPiece, secondFileInit, castleRank,
                secondFileTarget, castleRank
            );
        }
    }

    --m_moveIterator;
}

void MoveTreeManager::setTransitioningPiece(
    bool isUndo_, shared_ptr<Piece>& p_, int initialFile_, int initialRank_,
    int targetFile_, int targetRank_, shared_ptr<Piece>& captured_,
    int capturedFile_, int capturedRank_
)
{
    m_transitioningPiece.resetPieces();
    m_transitioningPiece.setTransitioningPiece(p_);
    m_transitioningPiece.setDestination({ui::getWindowXPos(targetFile_), ui::getWindowYPos(targetRank_)});
    m_transitioningPiece.setCurrPos({ui::getWindowXPos(initialFile_), ui::getWindowYPos(initialRank_)});
    m_transitioningPiece.setCapturedPiece(captured_, ui::getWindowXPos(capturedFile_), ui::getWindowYPos(capturedRank_));
    m_transitioningPiece.setUndo(isUndo_);
    m_transitioningPiece.setIsTransitioning(true);
    m_transitioningPiece.setIncrement();
}

void MoveTreeManager::setSecondTransitioningPiece(
    shared_ptr<Piece>& p_, int initialFile_, int initialRank_,
    int targetFile_, int targetRank_
)
{
    m_transitioningPiece.setSecondTransitioningPiece(p_);
    m_transitioningPiece.setSecondDestination({ui::getWindowXPos(targetFile_), ui::getWindowYPos(targetRank_)});
    m_transitioningPiece.setSecondCurrPos({ui::getWindowXPos(initialFile_), ui::getWindowYPos(initialRank_)});
    m_transitioningPiece.setSecondIsTransitioning(true);
    m_transitioningPiece.setSecondIncrement();
}
















std::vector<std::string> MoveTreeManager::tokenizePGN(const std::string& pgn_) 
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

void MoveTreeManager::parseAllTokens(
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
            goToPreviousMove(false, dummyArrows);
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
                goToPreviousMove(false, dummyArrows);
            }
            goToNextMove(false, 0, dummyArrows);
            moveCount_ = undoCount;
            continue;
        }

        addMoveToPGNTree(token);
        ++moveCount_;
        //getMoves().printTree();
    }
}


void MoveTreeManager::addMoveToPGNTree(const std::string& token_)
{   
    std::string moveToken = token_;

    std::cout <<"treating token : " << token_ << std::endl;

    std::vector<Move> allPossibleMoves = game.getAllCurrentlyAvailableMoves();
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

        Team currTeamTurn = game.getTurn();
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
    
    auto pMove = game.applyMoveOnBoardTesting(
        selectedMove.getMoveType(),
        std::make_pair(moveTargetFile, moveTargetRank),
        std::make_pair(moveInitialFile, moveInitialRank),
        selectedMove.getSelectedPiece());

    std::vector<Arrow> arrows;
    addMove(pMove, arrows); 
    game.updateBoardInfosAfterNewMove(selectedMove.getSelectedPiece(), pMove);
}

void MoveTreeManager::initializeMoveSequenceFromPNG(const std::string& pgn_)
{
    game.updateAllCurrentlyAvailableMoves();
    // Tokenize the PGN string 
    std::vector<std::string> tokens = tokenizePGN(pgn_);
    if (tokens.size() == 0) return;

    size_t index = 0;
    int moveCount = 0;
    std::stack<int> undoStack;
    parseAllTokens(tokens, index, moveCount, undoStack);
}