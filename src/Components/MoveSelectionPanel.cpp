#include "../../include/Components/MoveSelectionPanel.hpp"

void MoveSelectionPanel::handleTitleText() {
    shared_ptr<Font> f = RessourceManager::getFont("Arial.ttf");
    m_title.setFont(*f);
    m_title.setString("Select a variation");
    m_title.setCharacterSize(14);
    m_title.setFillColor(Color::Black);
    m_title.setPosition(m_panel.getPosition() + Vector2f(10.f,10.f));
}

void MoveSelectionPanel::handlePanelRectangle(int variationsNumber) {
    m_height = TOP_PANEL_HEIGHT + (variationsNumber * VARIATION_HEIGHT) + (2 * INNER_MARGIN);

    // draw all the panel
    m_panel.setSize(Vector2f(PANEL_WIDTH,m_height));
    m_panel.setFillColor(Color(211,211,211)); 
    m_panel.setPosition(WINDOW_SIZE + PANEL_SIZE / 3, PANEL_SIZE / 3);
}

void MoveSelectionPanel::handleSubPanels(int variationsNumber) {
    // draw top left rectangle and title text
    m_topRect.setSize(Vector2f(PANEL_WIDTH,TOP_PANEL_HEIGHT));
    m_topRect.setFillColor(Color(240, 248, 255));
    m_topRect.setPosition(m_panel.getPosition());

    // draw bottom rectangle for the set of variation (buttons)
    const int h = m_height - (2 * INNER_MARGIN) - TOP_PANEL_HEIGHT;
    Vector2f panelPos{INNER_MARGIN, TOP_PANEL_HEIGHT + INNER_MARGIN};
    m_variationsPanel.setSize(Vector2f(PANEL_WIDTH - (2 * INNER_MARGIN), h));
    m_variationsPanel.setFillColor(Color(240, 248, 255));
    m_variationsPanel.setPosition(m_panel.getPosition() + panelPos);
    m_variationsPanel.setOutlineColor(Color::Black);
    m_variationsPanel.setOutlineThickness(2.f);
}

void MoveSelectionPanel::drawMoveSelectionPanel(int variationsNumber) {
    handlePanelRectangle(variationsNumber);
    handleSubPanels(variationsNumber);
    handleTitleText();

    m_window.draw(m_panel);
    m_window.draw(m_topRect);
    m_window.draw(m_variationsPanel);
    m_window.draw(m_title);
}