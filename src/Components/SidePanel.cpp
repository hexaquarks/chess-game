#include "../../include/Components/SidePanel.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"
#include "../../include/Ressources/RessourceManager.hpp"

SidePanel::SidePanel(
    RenderWindow& window_, MoveList& moveList_, bool& b_
):
    m_window(window_), m_moveList(moveList_),
    m_showMoveSelectionPanel(b_)
{
}

void SidePanel::addMove(const Move& move_)
{
    // get the text coordinates information for a Move Box
    int moveListSize = m_moveList.getMoveListSize();
    int moveNumber = (moveListSize / 2) + 1;
    bool showNumber = moveListSize % 2 != 0;
    //string text = parseMove(move_, moveNumber, showNumber);
    string text = "temp";

    // construct the Move Box
    MoveBox moveBox(m_nextPos, text); // Make the text box
    moveBox.handleText(); // Create the Text, and pass the font ressource
    checkOutOfBounds(moveBox, 0); // check if object's width goes out of bounds and update
    m_nextPos.first += (moveBox.getScaledWidth()); // increment for next move box

    moveBoxes.push_back(moveBox);
    ++moveBoxCounter;
}

void SidePanel::goToNextRow(int height_)
{
    m_nextPos = {ui::g_BORDER_SIZE + 10, m_nextPos.second + height_ + 20};
}

void SidePanel::checkOutOfBounds(MoveBox& moveBox_, int offset_)
{
    int newPos = ui::g_WINDOW_SIZE + offset_ + m_nextPos.first + moveBox_.getTextBounds().width;
    if (newPos >= ui::g_WINDOW_SIZE + ui::g_PANEL_SIZE - ui::g_BORDER_SIZE)
    {
        goToNextRow(moveBox_.getTextBounds().height); // Change next position
        moveBox_.setPosition(m_nextPos); // Update the new position
    }
}

void SidePanel::handleMoveBoxClicked(const coor2d& mousePos_) const
{
    int newMoveIndex = 0;

    for(auto& moveBox : moveBoxes)
    {
        float width = moveBox.getScaledWidth();
        float height = moveBox.getScaledHeight();
        const auto [xPos, yPos] = moveBox.getPosition();

        int x = mousePos_.first - ui::g_WINDOW_SIZE;
        int y = mousePos_.second - ui::g_MENUBAR_HEIGHT;

        if ((x >= xPos && x < xPos + width) && (y >= yPos && y < yPos + height))
        {
            int currMoveIndex = m_moveList.getMoveListSize() - m_moveList.getIteratorIndex() -1;
            vector<Arrow> temp; // For testing
            if (newMoveIndex > currMoveIndex)
            {
                while (newMoveIndex > currMoveIndex)
                {
                    m_moveList.goToNextMove(false, std::nullopt, temp);
                    --newMoveIndex;
                }
            }
            else if (newMoveIndex < currMoveIndex)
            {
                while (newMoveIndex < currMoveIndex)
                {
                    m_moveList.goToPreviousMove(false, temp);
                    ++newMoveIndex;
                }
            }
            break;
        }
        ++newMoveIndex;
    }
}

void SidePanel::drawSquareBracket(coor2d& nextPos_, int offset_, bool open_) const
{
    nextPos_.first += offset_;

    auto font = RessourceManager::getFont("Arial.ttf");
    Text textsf;
    SFDrawUtil::drawTextSf(textsf, string(open_ ? "[" : "]"), *font, 28, Text::Bold, {240, 248, 255});

    Vector2f recSize(textsf.getGlobalBounds().width, textsf.getCharacterSize());
    RectangleShape rect;
    SFDrawUtil::drawRectangleSf(
        rect, (ui::g_WINDOW_SIZE + nextPos_.first),
        (ui::g_MENUBAR_HEIGHT + nextPos_.second), recSize, Color(50, 50, 50)
    );

    float positionalShift = ((g_BOX_HORIZONTAL_SCALE - 1.f) * rect.getLocalBounds().width)/2.f;
    rect.setScale(g_BOX_HORIZONTAL_SCALE, g_BOX_VERTICAL_SCALE);
    textsf.setPosition(
        ui::g_WINDOW_SIZE + nextPos_.first + positionalShift,
        ui::g_MENUBAR_HEIGHT + nextPos_.second - 4
    );

    m_window.draw(rect);
    m_window.draw(textsf);
    nextPos_.first -= offset_;
}

void SidePanel::drawFromNode(const shared_ptr<MoveTreeNode>& node_, int level_, int offset_, coor2d& nextPos_, const coor2d& mousePos)
{
    // Base case leaf
    if (!node_->m_move && node_->m_parent)
    {
        // Close the open bracket for sub variation
        if (offset_ != 0) drawSquareBracket(nextPos_, offset_ + 10, false);
        return;
    }

    // Draw the node
    if (node_->m_move)
    {
        nextPos_ = drawMove(*(node_->m_move), level_, offset_, nextPos_, mousePos);
    }

    // Recursive step
    for (int i = 0; i < node_->childNumber; ++i)
    {
        if (i == 0) drawFromNode(node_->m_children.at(0), level_+1, offset_, nextPos_, mousePos);
        else
        {
            ++m_row;
            nextPos_ = { g_INIT_WIDTH, g_INIT_HEIGHT + (m_row * g_ROW_HEIGHT) };
            drawSquareBracket(nextPos_, offset_ + g_HORIZONTAL_OFFSET - 10, true);
            drawFromNode(
                node_->m_children.at(i), level_+1,
                offset_ + g_HORIZONTAL_OFFSET, nextPos_, mousePos
            );
        }
    }
}

coor2d SidePanel::drawMove(const Move& move_, int level_, int offset_, coor2d& nextPos_, const coor2d& mousePos_)
{
    // Iterate through all the move list from begining to end
    // Get the text coordinates information for a Move Box
    int moveNumber = (level_ / 2) + 1;
    bool showNumber = level_ % 2 != 0;
    string text = parseMove(move_, moveNumber, showNumber);

    // Construct the Move Box
    nextPos_.first += offset_;
    MoveBox moveBox(nextPos_, text); // Make the text box
    moveBox.handleText(); // Create the Text, and pass the font ressource

    // CheckOutOfBounds(moveBox, offset); // check if object's width goes out of bounds and update
    // m_nextPos.first += (moveBox.getScaledWidth()); // increment for next move box

    moveBox.handleRectangle();
    if (!m_showMoveSelectionPanel)
    {
        // Change the color of the Move Box if it is howered
        if (moveBox.isHowered(mousePos_)) moveBox.setIsSelected();
        else moveBox.setDefault();
    }
    m_window.draw(moveBox.getRectangle());
    m_window.draw(moveBox.getTextsf());

    return {nextPos_.first += (moveBox.getScaledWidth()-offset_), nextPos_.second};
}

void SidePanel::drawMoves(const coor2d& mousePos_)
{
    shared_ptr<MoveTreeNode> root = m_moveList.getMoves().getRoot();
    coor2d coors = {g_INIT_WIDTH, g_INIT_HEIGHT};
    drawFromNode(root, 0, 0, coors, mousePos_);
    m_row = 0; // Reset to 0 for next iteration
    // if (moveBoxes.size() == 0) return; // No moves added yet, return

    // int counter = 0;
    // for (auto& moveBox : moveBoxes) {
    //     moveBox.handleRectangle();

    //     if (!m_showMoveSelectionPanel) {
    //         // Change the color of the Move Box if it is howered
    //         if (moveBox.isHowered(mousePos_)) moveBox.setIsSelected();
    //         else moveBox.setDefault();
    //     }

    //     // Change the color of te Move Box if it is represents the current move
    //     int currMoveIndex = m_moveList.getMoveListSize() - m_moveList.getIteratorIndex() -1;
    //     if (counter == currMoveIndex) moveBox.setIsCurrentMove();

    //     ++counter;
    //     m_window.draw(moveBox.getRectangle());
    //     m_window.draw(moveBox.getTextsf());
    // }
    // resetNextPos();
}
