#include "../include/SidePanel.hpp"

void SidePanel::addMove(MoveList& moves, Move& move) {
    int moveListSize = moves.getMoveList().size();
    bool showNumber = moveListSize % 2 != 0;

    string text = parseMove(move, (moveListSize/2) + 1, showNumber);

    MoveBox moveBox(m_nextPos, text); // Make the text box
    moveBox.handleText(); // Create the Text, and pass the font ressource
    checkOutOfBounds(moveBox); // check if object's width goes out of bounds and update
    m_nextPos.first += (10 + moveBox.getTextBounds().width); // increment for next move box

    moveBoxes.emplace_back(moveBox);
}

pair<char,int> SidePanel::findLetterCoord(coor2d target) {
    char letter = letters.at(target.first);
    return make_pair(letter, 8-target.second);
}

string SidePanel::parseMove(Move& move, int moveNumber, bool showNumber) {
    string text = (showNumber)? to_string(moveNumber) + "." : " ";
    MoveType moveType = move.getMoveType();

    // side cases
    if (moveType == MoveType::CASTLE_KINGSIDE) {
        return text + "0-0";
    }
    if (moveType == MoveType::CASTLE_QUEENSIDE) {
        return text + "0-0-0";
    }

    coor2d coord = move.getTarget();
    pair<char, int> letterCoord = findLetterCoord(coord);
    string letterCoordString = (1, letterCoord.first) + to_string(letterCoord.second);;

    switch (move.getSelectedPiece()->getType()) {
        case PieceType::PAWN:
            // TODO fix promotion
            if(moveType == MoveType::CAPTURE || moveType == MoveType::ENPASSANT){
                text += string(1, letters.at(coord.first)) + "x";
                return text + letterCoordString;
            }
            break;
        case PieceType::KNIGHT:
            text += "N";
            break;
        case PieceType::BISHOP:
            text += "B";
            break;
        case PieceType::ROOK:
            text += "R";
            break;
        case PieceType::QUEEN:
            text += "Q";
            break;
        case PieceType::KING:
            text += "k";
            break;
    }
    return text + string((moveType == MoveType::CAPTURE)? "x" : "") + letterCoordString;
}

void SidePanel::goToNextRow(int height) {
     m_nextPos = {BORDER_SIZE + 10, m_nextPos.second + height + 20};
}

void SidePanel::checkOutOfBounds(MoveBox& moveBox) {
    int newPos = WINDOW_SIZE + m_nextPos.first + moveBox.getTextBounds().width;
    if (newPos >= WINDOW_SIZE + PANEL_SIZE - BORDER_SIZE) {
        goToNextRow(moveBox.getTextBounds().height); // change next position 
        moveBox.setPosition(m_nextPos); // update the new position
    }
}

void SidePanel::drawMoves(coor2d& mousePos) {
    if (moveBoxes.size() == 0 ) return; // no moves added yet, return

    for (auto& moveBox : moveBoxes) {
        moveBox.handleRectangle();

        if(moveBox.isHowered(mousePos)) moveBox.setIsSelected(); 
        else moveBox.setDefault();

        m_window.draw(moveBox.getRectangle());
        m_window.draw(moveBox.getTextsf());

    }
    // resetNextPos();
}

coor2d SidePanel::findNextAvailableSpot() {
    return make_pair(0,0);
}