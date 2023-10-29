#include "../../include/UI/MoveSelectionPanel.hpp"
#include "../../include/Utilities/SFDrawUtil.hpp"
#include "../../include/Ressources/RessourceManager.hpp"

MoveSelectionPanel::MoveSelectionPanel(
    RenderWindow& window_,
    SidePanel& panel_)
: m_window(window_)
{
}

void MoveSelectionPanel::handleTitleText()
{
    auto font = RessourceManager::getFont("Arial.ttf");
    SFDrawUtil::drawTextSf(
        m_title, "Select a variation", *font, 14,
        Text::Style::Regular, Color::Black
    );
    m_title.setPosition(m_panel.getPosition() + Vector2f(10.f, 10.f));
}

void MoveSelectionPanel::handlePanelRectangle()
{
    m_height = g_TOP_PANEL_HEIGHT + (m_numberOfVariations * g_VARIATION_HEIGHT) + (2 * g_INNER_MARGIN);

    // Draw all the panel
    SFDrawUtil::drawRectangleSf(
        m_panel, ui::g_WINDOW_SIZE + ui::g_PANEL_SIZE / 3, ui::g_PANEL_SIZE / 3,
        Vector2f(g_PANEL_WIDTH, m_height), g_GRAY
    );
}

void MoveSelectionPanel::handleSubPanels()
{
    // Draw top left rectangle and title text
    SFDrawUtil::drawRectangleSf(
        m_topRect, m_panel.getPosition().x, m_panel.getPosition().y,
        Vector2f(g_PANEL_WIDTH, g_TOP_PANEL_HEIGHT), g_LIGHT_WHITE
    );

    // Draw bottom rectangle for the set of variation (buttons)
    const int h = m_height - (2 * g_INNER_MARGIN) - g_TOP_PANEL_HEIGHT;
    Vector2f panelPos{ g_INNER_MARGIN, g_TOP_PANEL_HEIGHT + g_INNER_MARGIN };
    Vector2f netPos = m_panel.getPosition() + panelPos;
    SFDrawUtil::drawRectangleSf(m_variationsPanel, netPos.x, netPos.y,
                                Vector2f(g_PANEL_WIDTH - (2 * g_INNER_MARGIN), h), g_LIGHT_WHITE);
    m_variationsPanel.setOutlineColor(Color::Black);
    m_variationsPanel.setOutlineThickness(2.f);
}

void MoveSelectionPanel::handleVariations(vector<std::string>& variations_)
{
    // No need to redraw content.
    if (m_variationRectangles.size() != 0 || m_variationTexts.size() != 0) return;

    size_t counter = 0;
    for (const auto& text : variations_)
    {
        // Draw the rectangle container for the variation text
        RectangleShape variationRect{Vector2f(g_PANEL_WIDTH - 2 * g_INNER_MARGIN, g_VARIATION_HEIGHT)};
        Vector2f newPos{ 0, float(counter) * g_VARIATION_HEIGHT };
        variationRect.setFillColor((counter == m_selectionIndex)? g_BLUE: g_LIGHT_WHITE);
        variationRect.setPosition(m_variationsPanel.getPosition() + newPos);
        variationRect.setOutlineColor(Color::Black);
        variationRect.setOutlineThickness(2.f);
        m_variationRectangles.push_back(variationRect);

        // Write the variation texts
        Text variationText;
        shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
        SFDrawUtil::drawTextSf(variationText, text, *f, 16, Text::Regular, sf::Color((counter == m_selectionIndex)? sf::Color::Black : sf::Color::Black));
        variationText.setPosition(variationRect.getPosition() + Vector2f(5.f,5.f));
        m_variationTexts.push_back(variationText);

        ++counter;
    }
}

void MoveSelectionPanel::drawMoveSelectionPanel(MoveTree::Iterator& it_)
{
    // construct the strings
    int moveListSize = it_.getNodeLevel() + 2; // +2 because initial is nullptr
    int moveNumber = (moveListSize / 2) + 1;
    bool showNumber = moveListSize % 2 != 0;
    vector<string> variations;

    for (size_t i = 0; i < it_.get()->childNumber; ++i)
    {
        variations.push_back(
            parseMove(
                *(it_.get()->m_children.at(i)->m_move), moveNumber, showNumber, true
            )
        );
    }
    // Set the number of variations locally
    m_numberOfVariations = variations.size();

    handlePanelRectangle();
    handleSubPanels();
    handleVariations(variations);
    handleTitleText();

    m_window.draw(m_panel);
    m_window.draw(m_topRect);
    m_window.draw(m_title);
    
    for (const auto& rect: m_variationRectangles) m_window.draw(rect);
    for (const auto& text: m_variationTexts) m_window.draw(text);
}

bool MoveSelectionPanel::isHowered(const coor2d& mousePos_) const
{
    int x = m_panel.getPosition().x;
    int y = m_panel.getPosition().y;

    return (
        mousePos_.first >= x &&
        mousePos_.first <= x + m_panel.getLocalBounds().width &&
        mousePos_.second >= y &&
        mousePos_.second <= y + m_panel.getLocalBounds().height
    );
}

void MoveSelectionPanel::goToNextVariation()
{
    // set the blue variation back to normal color
    m_variationRectangles.at(m_selectionIndex).setFillColor(g_LIGHT_WHITE);

    // increment the selected variation
    if (m_selectionIndex != m_numberOfVariations - 1) ++m_selectionIndex;

    // Set the new variation to selected color
    m_variationRectangles.at(m_selectionIndex).setFillColor(g_BLUE);
}

void MoveSelectionPanel::goToPreviousVariation()
{
    // set the blue variation back to normal color
    m_variationRectangles.at(m_selectionIndex).setFillColor(g_LIGHT_WHITE);

    // decrement the selected variation
    if (m_selectionIndex != 0) --m_selectionIndex;

    // Set the new variation to selected color
    m_variationRectangles.at(m_selectionIndex).setFillColor(g_BLUE);
}

void MoveSelectionPanel::reset()
{
    m_variationRectangles.clear();
    m_variationTexts.clear();
}
