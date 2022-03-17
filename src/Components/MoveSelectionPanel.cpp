#include "../../include/Components/MoveSelectionPanel.hpp"

void MoveSelectionPanel::handleTitleText() {
    shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
    m_title.setFont(*f);
    m_title.setString("Select a variation");
    m_title.setCharacterSize(14);
    m_title.setFillColor(Color::Black);
    m_title.setPosition(m_panel.getPosition() + Vector2f(10.f,10.f));
}

void MoveSelectionPanel::handlePanelRectangle() {
    m_height = TOP_PANEL_HEIGHT + (m_numberOfVariations * VARIATION_HEIGHT) + (2 * INNER_MARGIN);

    // draw all the panel
    m_panel.setSize(Vector2f(PANEL_WIDTH, m_height));
    m_panel.setFillColor(GRAY); 
    m_panel.setPosition(WINDOW_SIZE + PANEL_SIZE / 3, PANEL_SIZE / 3);
}

void MoveSelectionPanel::handleSubPanels() {
    // draw top left rectangle and title text
    m_topRect.setSize(Vector2f(PANEL_WIDTH,TOP_PANEL_HEIGHT));
    m_topRect.setFillColor(LIGHT_WHITE);
    m_topRect.setPosition(m_panel.getPosition());

    // draw bottom rectangle for the set of variation (buttons)
    const int h = m_height - (2 * INNER_MARGIN) - TOP_PANEL_HEIGHT;
    Vector2f panelPos{INNER_MARGIN, TOP_PANEL_HEIGHT + INNER_MARGIN};
    m_variationsPanel.setSize(Vector2f(PANEL_WIDTH - (2 * INNER_MARGIN), h));
    m_variationsPanel.setFillColor(LIGHT_WHITE);
    m_variationsPanel.setPosition(m_panel.getPosition() + panelPos);
    m_variationsPanel.setOutlineColor(Color::Black);
    m_variationsPanel.setOutlineThickness(2.f);
}

void MoveSelectionPanel::handleVariations(vector<std::string>& variations) { 
    int counter = 0;
    for (auto& text : variations) {
        // Draw the rectangle container for the variation text
        RectangleShape variationRect{Vector2f(PANEL_WIDTH - 2 * INNER_MARGIN, VARIATION_HEIGHT)};
        Vector2f newPos{0, float(counter) * VARIATION_HEIGHT};
        variationRect.setFillColor((counter == m_selectionIndex)? BLUE: LIGHT_WHITE);
        variationRect.setPosition(m_variationsPanel.getPosition() + newPos);
        variationRect.setOutlineColor(Color::Black);
        variationRect.setOutlineThickness(2.f);
        m_variationRectangles.emplace_back(variationRect);

        // Write the variation texts
        Text variationText;
        shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
        variationText.setFont(*f);
        variationText.setString(text);
        variationText.setCharacterSize(16);
        variationText.setFillColor((counter == m_selectionIndex)? Color::White : Color::Black);
        variationText.setPosition(variationRect.getPosition() + Vector2f(5.f,5.f));
        m_variationTexts.emplace_back(variationText);

        ++counter;
    }
}

void MoveSelectionPanel::drawMoveSelectionPanel(MoveTree::Iterator& it) {
    // construct the strings
    int moveListSize = it.getNodeLevel() + 2;  // +2 because initial is nullptr
    int moveNumber = (moveListSize / 2) + 1;
    bool showNumber = moveListSize % 2 != 0;
    vector<string> variations; 
    
    for (int i = 0; i < it.get()->childNumber; ++i) {
        variations.emplace_back(
            m_sidePanel.parseMove(
                *(it.get()->m_children.at(i)->m_move), moveNumber, showNumber, true
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

void MoveSelectionPanel::goToNextVariation() {
    // set the blue variation back to normal color
    m_variationRectangles.at(m_selectionIndex).setFillColor(LIGHT_WHITE);

    // increment the selected variation
    if (m_selectionIndex != m_numberOfVariations-1) ++m_selectionIndex;
    
    // Set the new variation to selected color
    m_variationRectangles.at(m_selectionIndex).setFillColor(BLUE);
}

void MoveSelectionPanel::goToPreviousVariation() {
    // set the blue variation back to normal color
    m_variationRectangles.at(m_selectionIndex).setFillColor(LIGHT_WHITE);
    
    // decrement the selected variation
    if (m_selectionIndex != 0) --m_selectionIndex;
    
    // Set the new variation to selected color
    m_variationRectangles.at(m_selectionIndex).setFillColor(BLUE);
}

void MoveSelectionPanel::reset() {
    // m_variationRectangles.clear();
    m_variationTexts.clear();
}