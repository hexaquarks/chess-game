#include "../../include/Components/SidePanel.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"
#include "../../include/Ressources/RessourceManager.hpp"

namespace 
{
    bool moveBoxIsOutOfBounds(const coor2d& absolutePosition_, const MoveBox& moveBox_)
    {
        return absolutePosition_.first + moveBox_.getScaledWidth() > ui::g_PANEL_SIZE - ui::g_BORDER_SIZE;
    }

    void updateRowInAllRemainingMoves(std::vector<MoveInfo>& moveTreeInfo_, size_t idx_) 
    {
        // -1 here because we already increased the row of the current 
        // moveInfo structure in handleOutOfBounds function.
        int currRow = moveTreeInfo_[idx_].m_row - 1;
        while (moveTreeInfo_[idx_].m_row == currRow) ++idx_;

        for (; idx_ < moveTreeInfo_.size(); ++idx_) ++moveTreeInfo_[idx_].m_row;
    }

    void handleMoveBoxHovered(MoveBox& moveBox_, const coor2d& mousePos_)
    {
        moveBox_.setIsHovered(moveBox_.isHowered(mousePos_));
    }

    void handleMoveBoxIsCurrentMove(MoveBox& moveBox_, bool isActualCurrentMove_)
    {
        if (isActualCurrentMove_) moveBox_.setIsCurrentMove();
    }

    sf::Text createMovePrefixText(const std::string& prefixLetter_, sf::Font& font_)
    {
        sf::Text text;
        SFDrawUtil::drawTextSf(text, prefixLetter_, font_, 25, sf::Text::Bold, {240, 248, 255});

        return text;
    }

    sf::RectangleShape createMovePrefixRect(coor2d& position_, const sf::Vector2f& size_)
    {
        sf::RectangleShape rect;
        SFDrawUtil::drawRectangleSf(rect, ui::g_WINDOW_SIZE + position_.first,
                ui::g_MENUBAR_HEIGHT + position_.second, size_, sf::Color(50, 50, 50));

        float positionalShift = ((g_BOX_HORIZONTAL_SCALE - 1.f) * rect.getLocalBounds().width) / 2.f;
        rect.setScale(g_BOX_HORIZONTAL_SCALE, g_BOX_VERTICAL_SCALE);

        return rect;
    }
}

SidePanel::SidePanel(
    RenderWindow& window_, MoveTreeManager& moveList_, bool& b_
):
    m_window(window_), m_moveTreeManager(moveList_),
    m_showMoveSelectionPanel(b_)
{
}

void SidePanel::addMove(const MoveInfo& move_)
{
    string text = move_.m_content;

    // construct the Move Box
    MoveBox moveBox(m_nextPos, text); // Make the text box
    moveBox.handleText(); // Create the Text, and pass the font ressource
    //checkOutOfBounds(moveBox, 0); // check if object's width goes out of bounds and update
    m_nextPos.first += (moveBox.getScaledWidth()); // increment for next move box

    moveBoxes.push_back(moveBox);
    ++moveBoxCounter;
}

void SidePanel::initializeMoveBoxCoodinates(const MoveInfo& move_, coor2d& absolutePosition_)
{
    if (move_.m_row == m_previousRow)
    {
        absolutePosition_ = m_nextPos;
    }
    else 
    {
        m_previousRow = move_.m_row;
        absolutePosition_.first = ui::g_BORDER_SIZE + 10 + move_.m_indentLevel * ui::g_INDENT_WIDTH;
    }

    // Adjust the vertical position based on the level
    absolutePosition_.second = move_.m_row * ui::g_LINE_HEIGHT + ui::g_SIDE_PANEL_TOP_OFFSET;
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

void SidePanel::handleMoveBoxOutOfBounds(
    coor2d& absolutePosition_, 
    MoveBox& moveBox_,
    MoveInfo& moveInfo_)
{
    // Go to next row
    ++moveInfo_.m_row;
    initializeMoveBoxCoodinates(moveInfo_, absolutePosition_);

    // Update the information for display
    moveBox_.setPosition(absolutePosition_);
    moveBox_.handleRectangle();
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
            int currMoveIndex = m_moveTreeManager.getMoveListSize() - m_moveTreeManager.getIteratorIndex() -1;
            vector<Arrow> temp; // For testing
            if (newMoveIndex > currMoveIndex)
            {
                while (newMoveIndex > currMoveIndex)
                {
                    m_moveTreeManager.goToNextMove(false, std::nullopt, temp);
                    --newMoveIndex;
                }
            }
            else if (newMoveIndex < currMoveIndex)
            {
                while (newMoveIndex < currMoveIndex)
                {
                    m_moveTreeManager.goToPreviousMove(false, temp);
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

void SidePanel::drawMovePrefix(const std::string& prefixLetter_, coor2d& position_)
{
    auto font = RessourceManager::getFont("Arial.ttf");
    auto text = createMovePrefixText(prefixLetter_, *font);
    auto rect = createMovePrefixRect(position_, { text.getGlobalBounds().width, static_cast<float>(text.getCharacterSize()) });

    float positionalShift = ((g_BOX_HORIZONTAL_SCALE - 1.f) * rect.getLocalBounds().width) / 2.f;
    text.setPosition(ui::g_WINDOW_SIZE + position_.first + positionalShift,
                      ui::g_MENUBAR_HEIGHT + position_.second);

    m_window.draw(rect);
    m_window.draw(text);

    // Update the next position to draw
    position_.first += rect.getGlobalBounds().width;
}

void SidePanel::drawMove(
    std::vector<MoveInfo>& moveTreeInfo_, 
    size_t idx_,
    const coor2d& mousePos_, 
    bool isActualCurrentMove_)
{
    MoveInfo& move = moveTreeInfo_[idx_];

    coor2d absolutePosition;
    // We first initialize the moveBox's top left coordinates based off
    // The MoveInfo's indentation level and row.
    initializeMoveBoxCoodinates(move, absolutePosition);

    // For subvariations we must draw the letter and number prefix.
    if (move.m_letterPrefix.has_value())
    {
        drawMovePrefix(move.m_letterPrefix.value(), absolutePosition);
    }

    // Construct the Move Box
    MoveBox moveBox(absolutePosition, move.m_content); // Make the text box
    moveBox.handleText(); // Create the Text, and pass the font resource
    moveBox.handleRectangle(); // Create the Rectangle to display.

    if (moveBoxIsOutOfBounds(absolutePosition, moveBox))
    {
        handleMoveBoxOutOfBounds(absolutePosition, moveBox, move);
        updateRowInAllRemainingMoves(moveTreeInfo_, idx_);
    }
    handleMoveBoxHovered(moveBox, mousePos_);
    handleMoveBoxIsCurrentMove(moveBox, isActualCurrentMove_);

    m_window.draw(moveBox.getRectangle());
    m_window.draw(moveBox.getTextsf());

    coor2d realDimensionsOfCurrentMoveBox = {
        static_cast<int>(moveBox.getScaledWidth()), 
        static_cast<int>(moveBox.getScaledHeight())};

    // Update the next position to draw
    m_nextPos.first = absolutePosition.first + realDimensionsOfCurrentMoveBox.first;
    m_nextPos.second = absolutePosition.second;
}


void SidePanel::drawMoves(std::vector<MoveInfo> moveTreeInfo_, const coor2d& mousePos_)
{
    // Reset the initial drawing position
    m_nextPos = {ui::g_BORDER_SIZE + 10, 0};

    size_t idx = 0;
    for (const MoveInfo& moveInfo : moveTreeInfo_)
    {
        const bool isActualCurrentMove = moveInfo.m_movePtr == m_moveTreeManager.getIterator()->m_move.get();
        drawMove(moveTreeInfo_, idx, mousePos_, isActualCurrentMove);
        ++idx;
    }
}
