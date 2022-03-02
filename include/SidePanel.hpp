#pragma once
using namespace std;

constexpr uint32_t MENUBAR_HEIGHTT = 30; // temporary fix
constexpr uint32_t PANEL_SIZE = 640;
constexpr uint32_t BORDER_SIZE = 5;
constexpr uint32_t SOUTH_PANEL_HEIGHT = MENUBAR_HEIGHTT;
constexpr uint32_t MAIN_PANEL_HEIGHT = PANEL_SIZE - SOUTH_PANEL_HEIGHT;

class SidePanel {
    // vector<Row> rows;

    public:
    SidePanel(){};

    void getLocalCoordinates();
};