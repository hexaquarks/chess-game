#pragma once

#include "Ressources/RessourceManager.hpp"
#include "Utilities/PieceTransition.hpp"
#include "Components/MenuButton.hpp"
#include "Components/Board.hpp"
#include "Utilities/Arrow.hpp"
#include "Utilities/MoveTree.hpp"
#include "Utilities/MoveList.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/King.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Piece.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

class SidePanel; // Forward declaration
class MoveSelectionPanel; // Forward declaration

class GameThread
{
public:
    GameThread() = delete; // Delete constructor
    static void startGame();

    static void setTransitioningPiece(
        bool, shared_ptr<Piece>&, int, int, int, int,
        shared_ptr<Piece>&, int, int, PieceTransition&
    );

    static void setSecondTransitioningPiece(
        shared_ptr<Piece>&, int, int,
        int, int, PieceTransition&
    );
    static void highlightLastMove(RenderWindow&);

private:
    inline static Board board;
    inline static PieceTransition transitioningPiece;
    inline static MoveList moveList{board, transitioningPiece};
    // inline static MoveTree moveTree;
    inline static MoveTree::Iterator treeIterator = moveList.getIterator();
    inline static bool kingChecked = false;
    inline static bool noMovesAvailable = false;

    /* Graphical functions */
    static void handleKeyPressed(const Event&, MoveSelectionPanel&, vector<Arrow>&, bool&);

    inline static bool isFlipped = false;
};
