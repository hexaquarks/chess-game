#pragma once

#include "Board.hpp"
#include "Move.hpp"
#include "MoveTreeDisplayHandler.hpp"
#include "MoveTree.hpp"
#include "../Utilities/PieceTransition.hpp"

#include <list>
#include <functional>
#include <iterator>
#include <SFML/Graphics.hpp>
#include <stack>
#include <string>
#include <vector>
#include <sstream>

using namespace sf;

struct UndoRedoMoveInfo;

class MoveTreeManager
{
public:
    explicit MoveTreeManager(Board&);

    // This class is an identity and has ownership over the uniques
    // MoveTree and  MoveTreeDisplayHandler.
    MoveTreeManager(const MoveTreeManager&) = delete;
    MoveTreeManager& operator=(const MoveTreeManager&) = delete;

    ~MoveTreeManager() = default;

    MoveTree::Iterator getNewIterator() { return m_moves.begin(); }
    MoveTree::Iterator& getIterator() { return m_moveIterator; }
    const MoveTree& getMoves() const { return m_moves; }
    Board& getBoard() { return m_board; }
    MoveTreeDisplayHandler& getMoveTreeDisplayHandler() { return m_moveTreeDisplayHandler; }
    PieceTransition& getTransitioningPiece() { return m_transitioningPiece; }
    int getIteratorIndex() { return 0; }
    int getMoveListSize() const { return m_moves.getNumberOfMoves(); }

    void reset() { m_moves.clear(); m_moveIterator = m_moves.begin(); };
    bool goToPreviousMove(bool, vector<Arrow>&);
    bool goToNextMove(bool, const std::optional<size_t>&, vector<Arrow>&);
    void goToCurrentMove(vector<Arrow>& arrowList) { while (goToNextMove(false, std::nullopt, arrowList)); }
    void goToInitialMove(vector<Arrow>& arrowList) { while (goToPreviousMove(false, arrowList)); }
    void addMove(const shared_ptr<Move>&, vector<Arrow>& arrowList);

    bool isTransitionningPiece() { return m_transitioningPiece.getIsTransitioning(); }
    void setTransitioningPieceArrived() { m_transitioningPiece.setHasArrived(); }

private: 
    MoveTree m_moves;
    MoveTreeDisplayHandler m_moveTreeDisplayHandler{m_moves};
    MoveTree::Iterator m_moveIterator = m_moves.begin();
    Board& m_board;
    PieceTransition m_transitioningPiece;

    std::function<void(std::shared_ptr<Piece>&, const coor2d&, bool)> m_transitionPieceCallback;

    void applyMove(const shared_ptr<Move>&, bool, bool, vector<Arrow>&);
    void applyMove(bool, vector<Arrow>&);
    void undoMove(bool, vector<Arrow>&);

    void handleUndoMoveNormal(UndoRedoMoveInfo& undoRedoMoveInfo_);
    void handleUndoMoveCapture(UndoRedoMoveInfo& undoRedoMoveInfo_);
    void handleUndoMoveEnpassant(UndoRedoMoveInfo& undoRedoMoveInfo_);
    void handleUndoMoveCastle(UndoRedoMoveInfo& undoRedoMoveInfo_, std::pair<int, int>);
    void handleUndoMoveInitSpecial(UndoRedoMoveInfo& undoRedoMoveInfo_);
    void handleUndoMoveNewPiece(UndoRedoMoveInfo& undoRedoMoveInfo_);

    void handleRedoMoveNormal(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_);
    void handleRedoMoveCapture(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_);
    void handleRedoMoveEnpassant(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_);
    void handleRedoMoveCastle(UndoRedoMoveInfo& undoRedoMoveInfo_, std::pair<int, int>, bool addToList_);
    void handleRedoMoveInitSpecial(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_);
    void handleRedoMoveNewPiece(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_);

    void executeUndoRedoHandler(const std::map<MoveType, std::function<void()>>& undoRedoMap_, MoveType moveType_);

    // Piece transition handlers
    void setTransitioningPiece(TransitionInfo&& info_, bool isUndo_) {
        setTransitioningPieceImpl(std::move(info_), isUndo_);
    }
    void setSecondTransitioningPiece(TransitionInfo&& info_) {
        setTransitioningPieceImpl(std::move(info_), false /* isUndo_ */, true /* isSecondPiece_ */);
    }
    void setTransitioningPieceImpl(TransitionInfo&& info_, bool isUndo_, bool isSecondPiece_ = false);  
    void enableUndoPieceTransition(UndoRedoMoveInfo& undoRedoMoveInfo_);
    void enableRedoPieceTransition(UndoRedoMoveInfo& undoRedoMoveInfo_, bool addToList_, std::optional<std::shared_ptr<Piece>>& pPromotingPieceOpt_);
};