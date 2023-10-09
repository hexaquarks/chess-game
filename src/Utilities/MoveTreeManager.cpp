#include "../../include/Utilities/MoveTreeManager.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/GameThread.hpp"

#include <cassert> 
#include <iterator>

#define ENABLE_TRANSITION_IF_ENABLED(ENABLE, TRANSITION_FUNC, SECOND_TRANSITION_FUNC, ...) \
do { \
    if (ENABLE) { \
        TRANSITION_FUNC(__VA_ARGS__); \
        if (pSecondPiece) { \
            SECOND_TRANSITION_FUNC(pSecondPiece, secondFileInit, castleRank, secondFileTarget, castleRank); \
        } \
    } \
} while (0)

MoveTreeManager::MoveTreeManager(Board& board_): m_board(board_)
{
}

bool MoveTreeManager::goToPreviousMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    if (!m_moveIterator.isAtTheBeginning())
    {
        undoMove(enableTransition_, arrowList_);
        m_board.switchTurn();
        m_board.updateAllCurrentlyAvailableMoves();
        
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
        m_board.switchTurn();
        m_board.updateAllCurrentlyAvailableMoves();

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
    
    const int castleRank = (m_board.getTurn() == Team::WHITE)? 7: 0;
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
    m_board.resetBoardTile(prevFile, prevRank);
    arrowList_ = move_->getMoveArrows();

    switch (move_->getMoveType())
    {
        case MoveType::NORMAL:
            m_board.setBoardTile(file, rank, pSelectedPiece);
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
            auto pOldPiece = m_board.getBoardTile(file, rank);
            m_board.setBoardTile(file, rank, pSelectedPiece);
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
            m_board.resetBoardTile(oldCoors.first, oldCoors.second);
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(make_shared<Move>(*move_, pCapturedPiece, oldCoors), m_moveIterator);
            }
            break;

        case MoveType::CASTLE_KINGSIDE:
        {
            secondFileInit = 7;
            secondFileTarget = 5;
            pSecondPieceOpt = m_board.getBoardTile(secondFileInit, castleRank);
            m_board.resetBoardTile(secondFileInit, castleRank);
            m_board.setBoardTile(secondFileTarget, castleRank, pSecondPieceOpt.value());
            m_board.setBoardTile(6, castleRank, pSelectedPiece);
            m_board.setBoardTile(file, rank, pSelectedPiece);
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
            auto pSecondPiece = m_board.getBoardTile(secondFileInit, castleRank);
            m_board.resetBoardTile(secondFileInit, castleRank);
            m_board.setBoardTile(secondFileTarget, castleRank, pSecondPiece);
            m_board.setBoardTile(2, castleRank, pSelectedPiece);
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                coor2d target = {2, castleRank};
                move_->setTarget(target);
                m_moves.insertNode(make_shared<Move>(*move_, pSecondPiece), m_moveIterator);
            }
            break;
        }

        case MoveType::INIT_SPECIAL:
            m_board.setBoardTile(file, rank, pSelectedPiece);
            if (addToList_)
            {
                m_moves.insertNode(move_, m_moveIterator);
            }
            break;

        case MoveType::NEWPIECE:
        {
            auto pOldPiece = m_board.getBoardTile(file, rank);
            pPromotingPieceOpt = make_shared<Queen>(pSelectedPiece->getTeam(), rank, file);
            Piece::setLastMovedPiece(pPromotingPieceOpt.value());
            m_board.setBoardTile(file, rank, pPromotingPieceOpt.value());
            m_board.addPiece(pPromotingPieceOpt.value());
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

struct UndoMoveInfo
{
    int m_targetFile;
    int m_targetRank;
    int m_initFile;
    int m_initRank;
    std::shared_ptr<Piece> m_selectedPiece;
    std::shared_ptr<Piece> m_capturedPiece; 
    std::optional<coor2d> m_enPassantCapturedPieceInitCoords;

    // information for castling
    std::optional<int> m_secondPieceTargetFile;
    std::optional<int> m_secondPieceTargetRank;
    std::optional<int> m_secondPieceInitFile;
    std::optional<int> m_secondPieceInitRank;
    std::optional<std::shared_ptr<Piece>> m_castlingSecondPiece;
};

void MoveTreeManager::handleUndoMoveNormal( UndoMoveInfo& undoMoveInfo_)
{   
    shared_ptr<Piece> selectedPiece = undoMoveInfo_.m_selectedPiece;

    m_board.resetBoardTile(undoMoveInfo_.m_targetFile, undoMoveInfo_.m_targetRank);
    m_board.setBoardTile(undoMoveInfo_.m_initFile, undoMoveInfo_.m_initRank, selectedPiece);

    King* kingPiece = dynamic_cast<King*>(selectedPiece.get());
    if (kingPiece) selectedPiece->setAsFirstMovement();
}

void MoveTreeManager::handleUndoMoveCapture(UndoMoveInfo& undoMoveInfo_)
{
    auto capturedPiece = undoMoveInfo_.m_capturedPiece;

    // If we get here, there should be a captured piece available.
    assert(capturedPiece);

    m_board.setBoardTile(undoMoveInfo_.m_targetFile,undoMoveInfo_.m_targetRank, capturedPiece);
    m_board.setBoardTile(undoMoveInfo_.m_initFile, undoMoveInfo_.m_initRank, undoMoveInfo_.m_selectedPiece);
}

void MoveTreeManager::handleUndoMoveEnpassant(UndoMoveInfo& undoMoveInfo_)
{
    auto capturedPiece = undoMoveInfo_.m_capturedPiece;

    // If we get here, there should be a captured piece available.
    assert(capturedPiece);

    // The en passant infos should be set.
    assert(undoMoveInfo_.m_enPassantCapturedPieceInitCoords.has_value());
    int capturedFile = undoMoveInfo_.m_enPassantCapturedPieceInitCoords.value().first;
    int capturedRank = undoMoveInfo_.m_enPassantCapturedPieceInitCoords.value().second;

    m_board.resetBoardTile(undoMoveInfo_.m_targetFile,undoMoveInfo_.m_targetRank);
    m_board.setBoardTile(capturedFile, capturedRank, capturedPiece);
    m_board.setBoardTile(undoMoveInfo_.m_initFile, undoMoveInfo_.m_initRank, undoMoveInfo_.m_selectedPiece);
}

void MoveTreeManager::handleUndoMoveCastle(
    UndoMoveInfo& undoMoveInfo_,
    std::pair<int, int> rookFileBeforeAndAfterCastling_)
{
    const int castleRank = (undoMoveInfo_.m_selectedPiece->getTeam() == Team::WHITE)? 7: 0;

    // We define the target and initial squares to be clear
    auto& [rookFileAfterCastling, rookFileBeforeCastling] = rookFileBeforeAndAfterCastling_; 
    int kingFileAfterCastling = undoMoveInfo_.m_targetFile;
    int kingFileBeforeCastling = undoMoveInfo_.m_initFile;
    
    auto king = undoMoveInfo_.m_selectedPiece;
    auto rook = undoMoveInfo_.m_capturedPiece;

    // Undo the castling.
    m_board.resetBoardTile(rookFileAfterCastling, castleRank);
    m_board.resetBoardTile(kingFileAfterCastling, castleRank);
    m_board.setBoardTile(rookFileBeforeCastling, castleRank, rook);
    m_board.setBoardTile(kingFileBeforeCastling, castleRank, king);

    undoMoveInfo_.m_selectedPiece->setAsFirstMovement();
    undoMoveInfo_.m_capturedPiece->setAsFirstMovement();

    // Update the UndoMoveInfo struct
    undoMoveInfo_.m_secondPieceTargetFile = rookFileBeforeCastling;
    undoMoveInfo_.m_secondPieceTargetRank = castleRank;
    undoMoveInfo_.m_secondPieceInitFile = rookFileAfterCastling;
    undoMoveInfo_.m_secondPieceInitRank = castleRank;
    undoMoveInfo_.m_castlingSecondPiece = rook;
}

void MoveTreeManager::handleUndoMoveInitSpecial(UndoMoveInfo& undoMoveInfo_)
{
    auto selectedPiece = undoMoveInfo_.m_selectedPiece;

    m_board.resetBoardTile(undoMoveInfo_.m_targetFile, undoMoveInfo_.m_targetRank);
    m_board.setBoardTile(undoMoveInfo_.m_initFile, undoMoveInfo_.m_initRank, selectedPiece);

    selectedPiece->setAsFirstMovement();
}

void MoveTreeManager::handleUndoMoveNewPiece(UndoMoveInfo& undoMoveInfo_)
{
    shared_ptr<Piece> newPawn = make_shared<Pawn>(
        undoMoveInfo_.m_selectedPiece->getTeam(),
        undoMoveInfo_.m_initFile, 
        undoMoveInfo_.m_initRank);
        
    m_board.setBoardTile(undoMoveInfo_.m_targetFile, undoMoveInfo_.m_targetRank, undoMoveInfo_.m_capturedPiece);
    m_board.setBoardTile(undoMoveInfo_.m_initFile, undoMoveInfo_.m_initRank, newPawn);
}

void MoveTreeManager::executeUndoHandler(
    const std::map<MoveType, std::function<void()>>& keyMap_, 
    MoveType moveType_)
{
    auto it = keyMap_.find(moveType_);
    if (it != keyMap_.end()) it->second();
}

void MoveTreeManager::undoMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    shared_ptr<Move> move = m_moveIterator->m_move;
    if (!move) return;
    
    arrowList_ = move->getMoveArrows();

    // Defining the UndoMoveInfo
    UndoMoveInfo undoMoveInfo{
        move->getTarget().first,
        move->getTarget().second,
        move->getInit().first,
        move->getInit().second,
        move->getSelectedPiece(),
        move->getCapturedPiece(),
        move->getEnPassantCapturedPieceInitialPos()
    };

    int rookFileAfterCastlingKingSide = 5; 
    int rookFileBeforeCastlingKingSide = 7;  

    // TODO: Review design here. Ideally, this would be a static map, but 
    // it causes dangling references to members of undoMoveInfo. I tried 
    // wrapping it in a shared_ptr, making undoMoveInfo static, but with no
    // success. 
    const map<MoveType, std::function<void()>> undoMoveHandlers
    {
        { MoveType::NORMAL, [this, &undoMoveInfo] { handleUndoMoveNormal(undoMoveInfo); } },
        { MoveType::CAPTURE, [this, &undoMoveInfo] { handleUndoMoveCapture(undoMoveInfo); } },
        { MoveType::ENPASSANT, [this, &undoMoveInfo] { handleUndoMoveEnpassant(undoMoveInfo); } },
        { MoveType::CASTLE_KINGSIDE, [this, &undoMoveInfo] 
        { 
            int rookFileAfterCastlingKingSide = 5; 
            int rookFileBeforeCastlingKingSide = 7; 

            handleUndoMoveCastle(undoMoveInfo, std::make_pair(
                rookFileAfterCastlingKingSide,
                rookFileBeforeCastlingKingSide
            )); 
        } },
        { MoveType::CASTLE_QUEENSIDE, [this, &undoMoveInfo] 
        { 
            int rookFileAfterCastlingQueenSide = 3; 
            int rookFileBeforeCastlingQueenSide = 0; 

            handleUndoMoveCastle(undoMoveInfo, std::make_pair(
                rookFileAfterCastlingQueenSide,
                rookFileBeforeCastlingQueenSide
            )); 
        } },
        { MoveType::INIT_SPECIAL, [this, &undoMoveInfo] { handleUndoMoveInitSpecial(undoMoveInfo); } },
        { MoveType::NEWPIECE, [this, &undoMoveInfo] { handleUndoMoveNewPiece(undoMoveInfo); } }
    };

    executeUndoHandler(undoMoveHandlers, move->getMoveType());

    if (enableTransition_)
    {
        // Enable transition movement
        setTransitioningPiece(
            true, 
            undoMoveInfo.m_selectedPiece, 
            undoMoveInfo.m_targetFile, 
            undoMoveInfo.m_targetRank, 
            undoMoveInfo.m_initFile, 
            undoMoveInfo.m_initRank, 
            undoMoveInfo.m_capturedPiece,
            undoMoveInfo.m_enPassantCapturedPieceInitCoords.value_or(
                std::make_pair(undoMoveInfo.m_targetFile, undoMoveInfo.m_targetRank)
            ).first, 
            undoMoveInfo.m_enPassantCapturedPieceInitCoords.value_or(
                std::make_pair(undoMoveInfo.m_targetFile, undoMoveInfo.m_targetRank)
            ).second
        );

        if (undoMoveInfo.m_castlingSecondPiece) {
            setSecondTransitioningPiece(
                undoMoveInfo.m_castlingSecondPiece.value(), 
                undoMoveInfo.m_secondPieceInitFile.value(), 
                undoMoveInfo.m_secondPieceInitRank.value(),
                undoMoveInfo.m_secondPieceTargetFile.value(),
                undoMoveInfo.m_secondPieceTargetRank.value()
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
