#pragma once
#include <SFML/Graphics.hpp>
#include "../GameThread.hpp"
#include "../Utilities/MoveTree.hpp"
#include "SidePanel.hpp"
#include <string>

using namespace std;
using namespace sf;

inline constexpr int TOP_PANEL_HEIGHT = 50;
inline constexpr int PANEL_WIDTH = 200;
inline constexpr int VARIATION_HEIGHT = 50;
inline constexpr int INNER_MARGIN = 15;
inline const Color BLUE{102, 178 ,255};
inline const Color GRAY{211,211,211};
inline const Color LIGHT_WHITE{240, 248, 255};

class MoveSelectionPanel {
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

    public:
    MoveSelectionPanel(RenderWindow& window, SidePanel& panel)
    : m_window(window), m_sidePanel(panel) {};

    int getSelection() { return m_selectionIndex; }
    void handleTitleText();
    void handlePanelRectangle();
    void handleSubPanels();
    void handleVariations(vector<std::string>&);
    void drawMoveSelectionPanel(MoveTree::Iterator&);
    void goToNextVariation();
    void goToPreviousVariation();
    void reset();
};