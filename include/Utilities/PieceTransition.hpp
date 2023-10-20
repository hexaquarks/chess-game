#pragma once
#include "../Pieces/Piece.hpp"
#include "../Components/Board.hpp"
#include <vector>

typedef std::pair<int, int> coor2d;
constexpr uint16_t g_FPS = 60;

struct TransitionInfo {
    std::shared_ptr<Piece> m_piece;
    int m_initialFile;
    int m_initialRank;
    int m_targetFile;
    int m_targetRank;
    std::optional<std::shared_ptr<Piece>> m_capturedPiece;
    std::optional<int> m_capturedFile;
    std::optional<int> m_capturedRank;
};

class PieceTransition
{
public:
    // Getters
    bool getIsTransitioning() const { return m_isTransitioning || m_second_isTransitioning; }
    bool getHasArrived() const {
        if (m_second_piece) return m_hasArrived && m_second_hasArrived;
        return m_hasArrived;
    }
    const std::shared_ptr<Piece>& getPiece() const { return m_piece; }
    const std::shared_ptr<Piece>& getSecondPiece() const { return m_second_piece; }
    const std::shared_ptr<Piece>& getCapturedPiece() const { return m_captured; }
    const std::shared_ptr<Piece>& getPromotingPiece() const { return m_promoting_piece; }
    const coor2d& getIncrement() const { return m_increment; }
    const coor2d& getSecondIncrement() const { return m_second_increment; }
    const coor2d& getDestination() const { return m_destination; }
    const coor2d& getSecondDestination() const { return m_second_destination; }
    const coor2d& getCurrPos() const { return m_currPos; }
    const coor2d& getSecondCurrPos() const { return m_second_currPos; }
    int getCapturedX() const { return capturedXPos; }
    int getCapturedY() const { return capturedYPos; }
    double getPercentageLeft() const { return distance_to_dest / original_dest; }
    bool isUndo() const { return undoMove; }

    // Setters
    void setCurrPos(coor2d&&);
    void setSecondCurrPos(coor2d&&);
    void setDestination(coor2d&& dest) { m_destination = dest; }
    void setSecondDestination(coor2d&& dest) { m_second_destination = dest; }
    void resetPieces() { m_piece.reset(); m_second_piece.reset(); }
    void setTransitioningPiece(std::shared_ptr<Piece>& p) { m_piece = p; }
    void setSecondTransitioningPiece(std::shared_ptr<Piece>& p) { m_second_piece = p; }
    void setIsTransitioning(bool b) { m_isTransitioning = b; }
    void setSecondIsTransitioning(bool b) { m_second_isTransitioning = b; }
    void setCapturedPiece(std::shared_ptr<Piece>&, int, int);
    void setPromotingPiece(std::shared_ptr<Piece>& p) { m_promoting_piece = p; }
    void setIncrement();
    void setSecondIncrement();
    void setHasArrived();
    void setUndo(bool value_) { undoMove = value_; }

    // Utilities
    void move();

private:
    std::shared_ptr<Piece> m_piece;
    std::shared_ptr<Piece> m_second_piece;
    std::shared_ptr<Piece> m_captured;
    std::shared_ptr<Piece> m_promoting_piece;
    coor2d m_currPos;
    coor2d m_second_currPos;
    coor2d m_destination;
    coor2d m_second_destination;
    coor2d m_increment;
    coor2d m_second_increment;
    int capturedXPos = 0;
    int capturedYPos = 0;
    bool m_hasArrived = false;
    bool m_second_hasArrived = false;
    bool m_isTransitioning = false;
    bool m_second_isTransitioning = false;
    double distance_to_dest = 0.0;
    double original_dest = 1.0;
    bool undoMove = true;

    void updateDistToDest();
    bool pieceIsInBounds() const;

    bool xArrived() const {
        return m_currPos.first == m_destination.first;
    }

    bool yArrived() const {
        return m_currPos.second == m_destination.second;
    }

    bool xSecondArrived() const {
        if (m_second_piece) return m_second_currPos.first == m_second_destination.first;
        return true;
    }

    bool ySecondArrived() const {
        if (m_second_piece) return m_second_currPos.second == m_second_destination.second;
        return true;
    }
};
