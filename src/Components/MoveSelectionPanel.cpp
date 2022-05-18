#include "../../include/Components/MoveSelectionPanel.hpp"
#include "../../include/Utilities/DrawableSf.hpp"

void MoveSelectionPanel::handleTitleText()
{
    shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
    DrawableSf::drawTextSf(m_title,"Select a variation", *f , 14, 
                           Text::Style::Regular, Color::Black);
    m_title.setPosition(m_panel.getPosition() + Vector2f(10.f, 10.f));
}

void MoveSelectionPanel::handlePanelRectangle()
{
    m_height = g_TOP_PANEL_HEIGHT + (m_numberOfVariations * g_VARIATION_HEIGHT) + (2 * g_INNER_MARGIN);

    // draw all the panel
    DrawableSf::drawRectangleSf(m_panel, (g_WINDOW_SIZE + g_PANEL_SIZE /  3), g_PANEL_SIZE / 3,
                                Vector2f(g_PANEL_WIDTH, m_height), g_GRAY);
}

void MoveSelectionPanel::handleSubPanels()
{
    // draw top left rectangle and title text
    DrawableSf::drawRectangleSf(m_topRect, m_panel.getPosition().x, m_panel.getPosition().y,    
                                Vector2f(g_PANEL_WIDTH, g_TOP_PANEL_HEIGHT), g_LIGHT_WHITE);

    // draw bottom rectangle for the set of variation (buttons)
    const int h = m_height - (2 * g_INNER_MARGIN) - g_TOP_PANEL_HEIGHT;
    Vector2f panelPos{ g_INNER_MARGIN, g_TOP_PANEL_HEIGHT + g_INNER_MARGIN };
    Vector2f netPos = m_panel.getPosition() + panelPos;
    DrawableSf::drawRectangleSf(m_variationsPanel, netPos.x, netPos.y,
                                Vector2f(g_PANEL_WIDTH - (2 * g_INNER_MARGIN), h), g_LIGHT_WHITE);
    m_variationsPanel.setOutlineColor(Color::Black);
    m_variationsPanel.setOutlineThickness(2.f);
}

void MoveSelectionPanel::handleVariations(vector<std::string>& variations_)
{
    int counter = 0;
    for (auto& text : variations_)
    {
        // Draw the rectangle container for the variation text
        RectangleShape variationRect{Vector2f(g_PANEL_WIDTH - 2 * g_INNER_MARGIN, g_VARIATION_HEIGHT)};
        Vector2f newPos{ 0, float(counter) * g_VARIATION_HEIGHT };
        variationRect.setFillColor((counter == m_selectionIndex)? g_BLUE: g_LIGHT_WHITE);
        variationRect.setPosition(m_variationsPanel.getPosition() + newPos);
        variationRect.setOutlineColor(Color::Black);
        variationRect.setOutlineThickness(2.f);
        m_variationRectangles.emplace_back(variationRect);

        // Write the variation texts
        Text variationText;
        shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
        DrawableSf::drawTextSf(variationText, text, *f, 16, Text::Regular, Color((counter == m_selectionIndex)? Color::White : Color::Black));
        variationText.setPosition(variationRect.getPosition() + Vector2f(5.f,5.f));
        m_variationTexts.emplace_back(variationText);

        ++counter;
    }
}

void MoveSelectionPanel::drawMoveSelectionPanel(MoveTree::Iterator& it_)
{
    // construct the strings
    int moveListSize = it_.getNodeLevel() + 2;  // +2 because initial is nullptr
    int moveNumber = (moveListSize / 2) + 1;
    bool showNumber = moveListSize % 2 != 0;
    vector<string> variations;
   
    for (int i = 0; i < it_.get()->childNumber; ++i)
    {
        variations.emplace_back(
            m_sidePanel.parseMove(
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
    for (auto& rect: m_variationRectangles) m_window.draw(rect);
    for (auto& text: m_variationTexts) m_window.draw(text);

    reset();
}

bool MoveSelectionPanel::isHowered(coor2d& mousePos_) const
{
    int x = m_panel.getPosition().x;
    int y = m_panel.getPosition().y;
   
    return (mousePos_.first >= x
     && mousePos_.first <= x + m_panel.getLocalBounds().width
     && mousePos_.second >= y
     && mousePos_.second <= y + m_panel.getLocalBounds().height);
}

void MoveSelectionPanel::goToNextVariation()
{
    // set the blue variation back to normal color
    m_variationRectangles.at(m_selectionIndex).setFillColor(g_LIGHT_WHITE);

    // increment the selected variation
    if (m_selectionIndex != m_numberOfVariations-1) ++m_selectionIndex;
   
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
    // m_variationRectangles.clear();
    m_variationTexts.clear();
}