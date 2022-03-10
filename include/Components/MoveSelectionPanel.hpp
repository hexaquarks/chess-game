#pragma once
#include <SFML/Graphics.hpp>
#include "../GameThread.hpp"

using namespace sf;
inline constexpr int TOP_PANEL_HEIGHT = 50;
inline constexpr int PANEL_WIDTH = 200;
inline constexpr int VARIATION_HEIGHT = 50;
inline constexpr int INNER_MARGIN = 15;

class MoveSelectionPanel {
    RenderWindow& m_window;
    Text m_title;
    RectangleShape m_panel;
    RectangleShape m_variationsPanel;
    RectangleShape m_topRect;
    int m_height;

    public:
    MoveSelectionPanel(RenderWindow& window): m_window(window) {};

    void handleTitleText();
    void handlePanelRectangle(int);
    void handleSubPanels(int);
    void drawMoveSelectionPanel(int);
};