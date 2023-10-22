#include "../../include/Utilities/MoveTreeManager.hpp"
#include "../../include/Utilities/PieceTransition.hpp"
#include "../../include/Utilities/UIConstants.hpp"
#include "../../include/GameThread.hpp"

#include <cassert> 
#include <iterator>

struct UndoRedoMoveInfo
{
    const std::shared_ptr<Move>& m_move;
    int m_targetFile;
    int m_targetRank;
    int m_initFile;
    int m_initRank;
    std::shared_ptr<Piece>& m_selectedPiece;
    std::shared_ptr<Piece>& m_capturedPiece; 

    // Optional edge-case information 
    std::optional<std::shared_ptr<Piece>> m_promotingPieceOpt;
    std::optional<coor2d> m_enPassantCapturedPieceInitCoords;

    // Optional information for castling
    std::optional<int> m_secondPieceTargetFile;
    std::optional<int> m_secondPieceTargetRank;
    std::optional<int> m_secondPieceInitFile;
    std::optional<int> m_secondPieceInitRank;
    std::optional<std::shared_ptr<Piece>> m_castlingSecondPiece;
};

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

void MoveTreeManager::handleRedoMoveNormal( UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_)
{   
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;
    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank, pSelectedPiece);

    if (addToList_) m_moves.insertNode(undoRedoMoveInfo_.m_move, m_moveIterator);
}

void MoveTreeManager::handleRedoMoveCapture(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_)
{
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;

    // Must pass by copy, otherwise tests break. TODO: investigate.
    auto pCapturedPiece = m_board.getBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank);

    // If we get here, there should be a captured piece available.
    assert(pSelectedPiece);
    assert(pCapturedPiece);

    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank, pSelectedPiece);

    if (addToList_) m_moves.insertNode(make_shared<Move>(*undoRedoMoveInfo_.m_move, pCapturedPiece), m_moveIterator);
}

void MoveTreeManager::handleRedoMoveEnpassant(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_)
{
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;
    auto& pCapturedPiece = undoRedoMoveInfo_.m_capturedPiece;

    assert(pSelectedPiece);
    assert(pCapturedPiece);    

    int capturedPieceFile = pCapturedPiece->getFile();
    int capturedPieceRank = pCapturedPiece->getRank();
    
    m_board.resetBoardTile(capturedPieceFile, capturedPieceRank);
    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank, pSelectedPiece);

    if (addToList_)
    {
        m_moves.insertNode(make_shared<Move>(
            *undoRedoMoveInfo_.m_move, 
            pCapturedPiece, 
            std::make_pair(capturedPieceFile, capturedPieceRank)), 
        m_moveIterator);
    }
}

// TODO : I realize memeory is heavily misshandled in piece movement. A copy is done 
// of the piece and then the previous piece pointer is reseted at the board tile. 
// A std::move() should probably be done? 
void MoveTreeManager::handleRedoMoveCastle(
    UndoRedoMoveInfo& undoRedoMoveInfo_,
    std::pair<int, int> rookFileBeforeAndAfterCastling_, 
    bool addToList_)
{   
    const int castleRank = (undoRedoMoveInfo_.m_selectedPiece->getTeam() == Team::WHITE)? 7: 0;

    // We define the target and initial squares to be clear
    auto& [rookFileAfterCastling, rookFileBeforeCastling] = rookFileBeforeAndAfterCastling_; 
    int kingFileAfterCastling = undoRedoMoveInfo_.m_targetFile;
    int kingFileBeforeCastling = undoRedoMoveInfo_.m_initFile;
    
    auto& king = undoRedoMoveInfo_.m_selectedPiece;
    auto rook = m_board.getBoardTile(rookFileBeforeCastling, castleRank);

    // Undo the castling.
    m_board.resetBoardTile(rookFileBeforeCastling, castleRank);
    m_board.setBoardTile(rookFileAfterCastling, castleRank, rook);
    m_board.setBoardTile(kingFileAfterCastling, castleRank, king);

    // Update the UndoRedoMoveInfo struct
    undoRedoMoveInfo_.m_secondPieceTargetFile = rookFileAfterCastling;
    undoRedoMoveInfo_.m_secondPieceTargetRank = castleRank;
    undoRedoMoveInfo_.m_secondPieceInitFile = rookFileBeforeCastling;
    undoRedoMoveInfo_.m_secondPieceInitRank = castleRank;
    undoRedoMoveInfo_.m_castlingSecondPiece = rook;

    if (addToList_) m_moves.insertNode(make_shared<Move>(*undoRedoMoveInfo_.m_move, rook), m_moveIterator);
}

void MoveTreeManager::handleRedoMoveInitSpecial(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_)
{
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;

    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank, pSelectedPiece);

    if (addToList_) m_moves.insertNode(undoRedoMoveInfo_.m_move, m_moveIterator);
}

void MoveTreeManager::handleRedoMoveNewPiece(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_)
{
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;
    auto pCapturedPiece = m_board.getBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank);

    std::shared_ptr<Piece> pPromotingPiece = make_shared<Queen>(
        undoRedoMoveInfo_.m_selectedPiece->getTeam(), 
        undoRedoMoveInfo_.m_targetFile, 
        undoRedoMoveInfo_.m_targetRank);
    Piece::setLastMovedPiece(pPromotingPiece);
    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank, pPromotingPiece);
    m_board.addPiece(pPromotingPiece);

    // Update the UndoRedoMove structure - we add the new piece that was created through promotion.
    undoRedoMoveInfo_.m_promotingPieceOpt = pPromotingPiece;

    if (addToList_) m_moves.insertNode(make_shared<Move>(*undoRedoMoveInfo_.m_move, pCapturedPiece), m_moveIterator);
}

void MoveTreeManager::applyMove(
    const shared_ptr<Move>& move_, 
    bool addToList_, 
    bool enableTransition_, 
    vector<Arrow>& arrowList_)
{
    if (!move_) return;
    
    // Set the current tile of the piece null. Necessary for navigating
    // back to current move through goToNextMove().
    const auto [prevFile, prevRank] = move_->getInit();
    m_board.resetBoardTile(prevFile, prevRank);
    arrowList_ = move_->getMoveArrows();
    std::optional<shared_ptr<Piece>> pPromotingPieceOpt;

    // Defining the UndoRedoMoveInfo
    UndoRedoMoveInfo undoRedoMoveInfo{
        move_,
        move_->getTarget().first,
        move_->getTarget().second,
        move_->getInit().first,
        move_->getInit().second,
        move_->getSelectedPiece(),
        move_->getCapturedPiece(),
        pPromotingPieceOpt,
        std::nullopt
    };

    // TODO: Review design here. Ideally, this would be a static map, but 
    // it causes dangling references to members of undoRedoMoveInfo. I tried 
    // wrapping it in a shared_ptr, tried making undoRedoMoveInfo static, but with no
    // success. 
    const map<MoveType, std::function<void()>> redoMoveHandlers
    {
        { MoveType::NORMAL, [this, &undoRedoMoveInfo, &addToList_] { handleRedoMoveNormal(undoRedoMoveInfo, addToList_); } },
        { MoveType::CAPTURE, [this, &undoRedoMoveInfo, &addToList_] { handleRedoMoveCapture(undoRedoMoveInfo, addToList_); } },
        { MoveType::ENPASSANT, [this, &undoRedoMoveInfo, &addToList_] { handleRedoMoveEnpassant(undoRedoMoveInfo, addToList_); } },
        { MoveType::CASTLE_KINGSIDE, [this, &undoRedoMoveInfo, &addToList_] 
        { 
            int rookFileAfterCastlingKingSide = 5; 
            int rookFileBeforeCastlingKingSide = 7; 

            handleRedoMoveCastle(undoRedoMoveInfo, std::make_pair(
                rookFileAfterCastlingKingSide,
                rookFileBeforeCastlingKingSide
            ), addToList_); 
        } },
        { MoveType::CASTLE_QUEENSIDE, [this, &undoRedoMoveInfo, &addToList_] 
        { 
            int rookFileAfterCastlingQueenSide = 3; 
            int rookFileBeforeCastlingQueenSide = 0; 

            handleRedoMoveCastle(undoRedoMoveInfo, std::make_pair(
                rookFileAfterCastlingQueenSide,
                rookFileBeforeCastlingQueenSide 
            ), addToList_); 
        } },
        { MoveType::INIT_SPECIAL, [this, &undoRedoMoveInfo, &addToList_] { handleRedoMoveInitSpecial(undoRedoMoveInfo, addToList_); } },
        { MoveType::NEWPIECE, [this, &undoRedoMoveInfo, &addToList_] { handleRedoMoveNewPiece(undoRedoMoveInfo, addToList_); } }
    };

    executeUndoRedoHandler(redoMoveHandlers, move_->getMoveType());
    if (enableTransition_) enableRedoPieceTransition(undoRedoMoveInfo, addToList_, pPromotingPieceOpt);
}

void MoveTreeManager::handleUndoMoveNormal( UndoRedoMoveInfo& undoRedoMoveInfo_)
{   
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;

    m_board.resetBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank);
    m_board.setBoardTile(undoRedoMoveInfo_.m_initFile, undoRedoMoveInfo_.m_initRank, pSelectedPiece);

    King* pKingPiece = dynamic_cast<King*>(pSelectedPiece.get());
    if (pKingPiece) pSelectedPiece->setAsFirstMovement();
}

void MoveTreeManager::handleUndoMoveCapture(UndoRedoMoveInfo& undoRedoMoveInfo_)
{
    auto& pCapturedPiece = undoRedoMoveInfo_.m_capturedPiece;
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;

    // If we get here, there should be a captured piece available.
    assert(pCapturedPiece);
    assert(pSelectedPiece);

    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile,undoRedoMoveInfo_.m_targetRank, pCapturedPiece);
    m_board.setBoardTile(undoRedoMoveInfo_.m_initFile, undoRedoMoveInfo_.m_initRank, undoRedoMoveInfo_.m_selectedPiece);
}

void MoveTreeManager::handleUndoMoveEnpassant(UndoRedoMoveInfo& undoRedoMoveInfo_)
{
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;
    auto& pCapturedPiece = undoRedoMoveInfo_.m_capturedPiece;

    // If we get here, there should be a captured piece available.
    assert(pCapturedPiece);
    assert(pSelectedPiece);

    // The en passant infos should be set.
    assert(undoRedoMoveInfo_.m_enPassantCapturedPieceInitCoords.has_value());
    int capturedFile = undoRedoMoveInfo_.m_enPassantCapturedPieceInitCoords.value().first;
    int capturedRank = undoRedoMoveInfo_.m_enPassantCapturedPieceInitCoords.value().second;

    m_board.resetBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank);
    m_board.setBoardTile(capturedFile, capturedRank, pCapturedPiece);
    m_board.setBoardTile(undoRedoMoveInfo_.m_initFile, undoRedoMoveInfo_.m_initRank, pSelectedPiece);
}

void MoveTreeManager::handleUndoMoveCastle(
    UndoRedoMoveInfo& undoRedoMoveInfo_,
    std::pair<int, int> rookFileBeforeAndAfterCastling_)
{
    const int castleRank = (undoRedoMoveInfo_.m_selectedPiece->getTeam() == Team::WHITE)? 7: 0;

    // We define the target and initial squares to be clear
    auto& [rookFileAfterCastling, rookFileBeforeCastling] = rookFileBeforeAndAfterCastling_; 
    int kingFileAfterCastling = undoRedoMoveInfo_.m_targetFile;
    int kingFileBeforeCastling = undoRedoMoveInfo_.m_initFile;
    
    auto& pKing = undoRedoMoveInfo_.m_selectedPiece;
    auto& pRook = undoRedoMoveInfo_.m_capturedPiece;

    // If we get here, there should be a king and a rook available.
    assert(pKing);
    assert(pRook);

    // Undo the castling.
    m_board.resetBoardTile(rookFileAfterCastling, castleRank);
    m_board.resetBoardTile(kingFileAfterCastling, castleRank);
    m_board.setBoardTile(rookFileBeforeCastling, castleRank, pRook);
    m_board.setBoardTile(kingFileBeforeCastling, castleRank, pKing);

    undoRedoMoveInfo_.m_selectedPiece->setAsFirstMovement();
    undoRedoMoveInfo_.m_capturedPiece->setAsFirstMovement();

    // Update the UndoRedoMoveInfo struct
    undoRedoMoveInfo_.m_secondPieceTargetFile = rookFileBeforeCastling;
    undoRedoMoveInfo_.m_secondPieceTargetRank = castleRank;
    undoRedoMoveInfo_.m_secondPieceInitFile = rookFileAfterCastling;
    undoRedoMoveInfo_.m_secondPieceInitRank = castleRank;
    undoRedoMoveInfo_.m_castlingSecondPiece = pRook;
}

void MoveTreeManager::handleUndoMoveInitSpecial(UndoRedoMoveInfo& undoRedoMoveInfo_)
{
    auto& pSelectedPiece = undoRedoMoveInfo_.m_selectedPiece;

    m_board.resetBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank);
    m_board.setBoardTile(undoRedoMoveInfo_.m_initFile, undoRedoMoveInfo_.m_initRank, pSelectedPiece);

    pSelectedPiece->setAsFirstMovement();
}

void MoveTreeManager::handleUndoMoveNewPiece(UndoRedoMoveInfo& undoRedoMoveInfo_)
{
    auto& pCapturePiece = undoRedoMoveInfo_.m_capturedPiece;
    assert(pCapturePiece);

    shared_ptr<Piece> pNewPawn = make_shared<Pawn>(
        undoRedoMoveInfo_.m_selectedPiece->getTeam(),
        undoRedoMoveInfo_.m_initFile, 
        undoRedoMoveInfo_.m_initRank);
        
    m_board.setBoardTile(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank, pCapturePiece);
    m_board.setBoardTile(undoRedoMoveInfo_.m_initFile, undoRedoMoveInfo_.m_initRank, pNewPawn);
}

void MoveTreeManager::executeUndoRedoHandler(
    const std::map<MoveType, std::function<void()>>& undoRedoMap_, 
    MoveType moveType_)
{
    auto it = undoRedoMap_.find(moveType_);
    if (it != undoRedoMap_.end()) it->second();
}

void MoveTreeManager::undoMove(bool enableTransition_, vector<Arrow>& arrowList_)
{
    shared_ptr<Move> move = m_moveIterator->m_move;
    if (!move) return;
    
    arrowList_ = move->getMoveArrows();

    // Defining the UndoRedoMoveInfo
    UndoRedoMoveInfo undoRedoMoveInfo{
        move,
        move->getTarget().first,
        move->getTarget().second,
        move->getInit().first,
        move->getInit().second,
        move->getSelectedPiece(),
        move->getCapturedPiece(),
        std::nullopt,
        move->getEnPassantCapturedPieceInitialPos()
    };

    // TODO: Review design here. Ideally, this would be a static map, but 
    // it causes dangling references to members of undoRedoMoveInfo. I tried 
    // wrapping it in a shared_ptr, making undoRedoMoveInfo static, but with no
    // success. 
    const map<MoveType, std::function<void()>> undoMoveHandlers
    {
        { MoveType::NORMAL, [this, &undoRedoMoveInfo] { handleUndoMoveNormal(undoRedoMoveInfo); } },
        { MoveType::CAPTURE, [this, &undoRedoMoveInfo] { handleUndoMoveCapture(undoRedoMoveInfo); } },
        { MoveType::ENPASSANT, [this, &undoRedoMoveInfo] { handleUndoMoveEnpassant(undoRedoMoveInfo); } },
        { MoveType::CASTLE_KINGSIDE, [this, &undoRedoMoveInfo] 
        { 
            int rookFileAfterCastlingKingSide = 5; 
            int rookFileBeforeCastlingKingSide = 7; 

            handleUndoMoveCastle(undoRedoMoveInfo, std::make_pair(
                rookFileAfterCastlingKingSide,
                rookFileBeforeCastlingKingSide
            )); 
        } },
        { MoveType::CASTLE_QUEENSIDE, [this, &undoRedoMoveInfo] 
        { 
            int rookFileAfterCastlingQueenSide = 3; 
            int rookFileBeforeCastlingQueenSide = 0; 

            handleUndoMoveCastle(undoRedoMoveInfo, std::make_pair(
                rookFileAfterCastlingQueenSide,
                rookFileBeforeCastlingQueenSide
            )); 
        } },
        { MoveType::INIT_SPECIAL, [this, &undoRedoMoveInfo] { handleUndoMoveInitSpecial(undoRedoMoveInfo); } },
        { MoveType::NEWPIECE, [this, &undoRedoMoveInfo] { handleUndoMoveNewPiece(undoRedoMoveInfo); } }
    };

    executeUndoRedoHandler(undoMoveHandlers, move->getMoveType());
    if (enableTransition_) enableUndoPieceTransition(undoRedoMoveInfo);

    --m_moveIterator;
}

void MoveTreeManager::enableRedoPieceTransition(
    UndoRedoMoveInfo& undoRedoMoveInfo_, 
    bool addToList_,
    std::optional<std::shared_ptr<Piece>>& pPromotingPieceOpt_)
{
    if (!addToList_ && undoRedoMoveInfo_.m_selectedPiece)
    {
        // Enable transition movement
        setTransitioningPiece(
            TransitionInfo{
                undoRedoMoveInfo_.m_selectedPiece,
                undoRedoMoveInfo_.m_initFile,
                undoRedoMoveInfo_.m_initRank,
                undoRedoMoveInfo_.m_targetFile,
                undoRedoMoveInfo_.m_targetRank,
                undoRedoMoveInfo_.m_capturedPiece,
                undoRedoMoveInfo_.m_targetFile,
                undoRedoMoveInfo_.m_targetRank
            },
            true
        );

        if (undoRedoMoveInfo_.m_castlingSecondPiece.has_value()) {
            setSecondTransitioningPiece(
                TransitionInfo{
                    undoRedoMoveInfo_.m_castlingSecondPiece.value(),
                    undoRedoMoveInfo_.m_secondPieceInitFile.value(),
                    undoRedoMoveInfo_.m_secondPieceInitRank.value(),
                    undoRedoMoveInfo_.m_secondPieceTargetFile.value(),
                    undoRedoMoveInfo_.m_secondPieceTargetRank.value(),
                }
            );
        }

        if (pPromotingPieceOpt_.has_value()) {
            getTransitioningPiece().setPromotingPiece(pPromotingPieceOpt_.value());
        }
    }
    // If the user manually drags and drops the king or castles through clicking,
    // We need to enable smooth transition for the rook. 
    // TODO: It seems that if we castle through clicking, only the rook will be 
    // transitioning smoothly - the king will teleport. Need to implement this.
    else if (undoRedoMoveInfo_.m_castlingSecondPiece.has_value())
    {
        // Enable rook sliding when user just castled
        // TODO try non-second piece transition if this doesnt work?
        setTransitioningPiece(
            TransitionInfo{
                undoRedoMoveInfo_.m_castlingSecondPiece.value(), 
                undoRedoMoveInfo_.m_secondPieceInitFile.value(), 
                undoRedoMoveInfo_.m_secondPieceInitRank.value(),
                undoRedoMoveInfo_.m_secondPieceTargetFile.value(),
                undoRedoMoveInfo_.m_secondPieceTargetRank.value(),
            }, 
            false
        );
    }
}

void MoveTreeManager::enableUndoPieceTransition(UndoRedoMoveInfo& undoRedoMoveInfo_)
{
    setTransitioningPiece(
        TransitionInfo{
            undoRedoMoveInfo_.m_selectedPiece,
            undoRedoMoveInfo_.m_targetFile,
            undoRedoMoveInfo_.m_targetRank,
            undoRedoMoveInfo_.m_initFile,
            undoRedoMoveInfo_.m_initRank,
            undoRedoMoveInfo_.m_capturedPiece,
            undoRedoMoveInfo_.m_enPassantCapturedPieceInitCoords.value_or(
                std::make_pair(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank)
            ).first,
            undoRedoMoveInfo_.m_enPassantCapturedPieceInitCoords.value_or(
                std::make_pair(undoRedoMoveInfo_.m_targetFile, undoRedoMoveInfo_.m_targetRank)
            ).second
        }, 
        true
    );

    if (undoRedoMoveInfo_.m_castlingSecondPiece.has_value()) {
        setSecondTransitioningPiece(
            TransitionInfo{
                undoRedoMoveInfo_.m_castlingSecondPiece.value(),
                undoRedoMoveInfo_.m_secondPieceInitFile.value(),
                undoRedoMoveInfo_.m_secondPieceInitRank.value(),
                undoRedoMoveInfo_.m_secondPieceTargetFile.value(),
                undoRedoMoveInfo_.m_secondPieceTargetRank.value(),
            }
        );
    }
}

void MoveTreeManager::setTransitioningPieceImpl(
    TransitionInfo&& info_,
     bool isUndo_, 
     bool isSecondPiece_) 
{
    coor2d destination = {ui::getWindowXPos(info_.m_targetFile), ui::getWindowYPos(info_.m_targetRank)};
    coor2d initialPos = {ui::getWindowXPos(info_.m_initialFile), ui::getWindowYPos(info_.m_initialRank)};

    if (isSecondPiece_) 
    {
        m_transitioningPiece.setSecondTransitioningPiece(info_.m_piece);
        m_transitioningPiece.setSecondDestination(std::move(destination));
        m_transitioningPiece.setSecondCurrPos(std::move(initialPos));
        m_transitioningPiece.setSecondIsTransitioning(true);
        m_transitioningPiece.setSecondIncrement();
    } 
    else 
    {
        m_transitioningPiece.resetPieces();
        m_transitioningPiece.setTransitioningPiece(info_.m_piece);
        m_transitioningPiece.setDestination(std::move(destination));
        m_transitioningPiece.setCurrPos(std::move(initialPos));
        m_transitioningPiece.setUndo(isUndo_);
        m_transitioningPiece.setIsTransitioning(true);
        m_transitioningPiece.setIncrement();
    }

    if (info_.m_capturedPiece) 
    {
        int capturedX = info_.m_capturedFile.value_or(info_.m_targetFile);
        int capturedY = info_.m_capturedRank.value_or(info_.m_targetRank);
        m_transitioningPiece.setCapturedPiece(
            info_.m_capturedPiece.value(), 
            ui::getWindowXPos(capturedX), 
            ui::getWindowYPos(capturedY));
    }
}
