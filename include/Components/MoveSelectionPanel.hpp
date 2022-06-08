#pragma once
#include "../GameThread.hpp"
#include "../Utilities/MoveTree.hpp"
#include "SidePanel.hpp"

#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

inline constexpr int g_TOP_PANEL_HEIGHT = 50;
inline constexpr int g_PANEL_WIDTH = 200;
inline constexpr int g_VARIATION_HEIGHT = 50;
inline constexpr int g_INNER_MARGIN = 15;
inline const Color g_BLUE = {102, 178, 255};
inline const Color g_GRAY = {211, 211, 211};
inline const Color g_LIGHT_WHITE = {240, 248, 255};

class MoveSelectionPanel
{
public:
    MoveSelectionPanel(RenderWindow& window, SidePanel& panel)
    : m_window(window), m_sidePanel(panel)
    {
    }

    bool isHowered(coor2d&) const;
    int getSelection() const { return m_selectionIndex; }
    void handleTitleText();
    void handlePanelRectangle();
    void handleSubPanels();
    void handleVariations(vector<std::string>&);
    void drawMoveSelectionPanel(MoveTree::Iterator&);
    void goToNextVariation();
    void goToPreviousVariation();
    void reset();

private:
    RenderWindow& m_window;
    SidePanel& m_sidePanel;
    Text m_title;
    RectangleShape m_panel;
    RectangleShape m_variationsPanel;
    RectangleShape m_topRect;
    vector<RectangleShape> m_variationRectangles;
    vector<Text> m_variationTexts;
    int m_height;
    int m_selectionIndex = 0;
    int m_numberOfVariations = 0;
    bool m_isHowered = false;

};