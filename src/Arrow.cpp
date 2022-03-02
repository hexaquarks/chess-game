#include <utility>
#include <algorithm> 
#include "../include/Arrow.hpp"
#include "../include/GameThread.hpp"

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

Arrow::Arrow(coor2d origin, coor2d destination, int rotation, string filename)
: m_origin(origin), m_destination(destination), m_rotation(rotation), m_filename(filename) {};

void Arrow::setOrigin(coor2d& origin) {
    // need to format the origin to be at the center of the tile
    m_origin.first = origin.first;
    m_origin.second = origin.second - MENUBAR_HEIGHT;
}

coor2d Arrow::getFormattedOrigin() {
    int x = (m_origin.first / CELL_SIZE) * CELL_SIZE + CELL_SIZE/2;
    int y = (m_origin.second / CELL_SIZE) * CELL_SIZE + CELL_SIZE/2 + MENUBAR_HEIGHT;
    return {x,y};
}

void Arrow::setDestination(coor2d& destination) {
    m_destination.first = destination.first;
    m_destination.second =  destination.second - MENUBAR_HEIGHT;

    // tile coordinates 
    m_dx = (m_destination.first/(int)CELL_SIZE - m_origin.first/(int)CELL_SIZE);
    m_dy = (m_destination.second/(int)CELL_SIZE - m_origin.second/(int)CELL_SIZE);
}

void Arrow::updateArrow() {
    // check if arrow is feasible 
    if ((m_dx == 0 && m_dy == 0)) return;
    if (m_dx != 0 || m_dy != 0) m_rotation = rotation[1 + sign(m_dx)][1 + sign(m_dy)];
    
    int size = std::max(abs(m_dx), abs(m_dy));
    if (size == 0) return; // do nothing, arrow too short
    if (checkOutOfBounds()) return; // do nothing, arrow out of window

    if (abs(abs(m_dx)-abs(m_dy)) == 1 && abs(m_dx) > 0 && abs(m_dy) > 0) checkKnightSquares();
    else {
        m_filename = "arrow_n" + to_string(size) + "x.png";
        m_isLArrow = false;
    }
}

bool Arrow::checkOutOfBounds() {
    return m_destination.first < 0 || m_destination.first > WINDOW_SIZE ||
    m_destination.second < 0 || m_destination.second > WINDOW_SIZE;
}

void Arrow::checkKnightSquares() {
    vector<coor2d>::iterator it_ur = urCoords.begin();
    vector<coor2d>::iterator it_ru = ruCoords.begin();

    while(it_ur != urCoords.end()) {
        if((*it_ur).first == m_dx && (*it_ur).second == m_dy){
            m_rotation = (it_ur - urCoords.begin())*90;
            m_filename = "arrow_Nur.png";
            break;
        }
        if((*it_ru).first == m_dx && (*it_ru).second == m_dy){
            m_rotation = (it_ru - ruCoords.begin())*90;
            m_filename = "arrow_Nru.png";
            break;
        }
        ++it_ru; ++it_ur;
    }
    m_isLArrow = true;
}

void Arrow::resetParameters() {
     m_origin.first = 0 , m_origin.second = 0;
     m_destination.first = 0 , m_destination.second = 0;
     m_dx = 0, m_dy = 0;
}

bool Arrow::isDrawable() {
    return abs(m_dx) > 0 || abs(m_dy) > 0;
}

bool Arrow::removeArrow(vector<Arrow>& arrows) {
    vector<Arrow>::iterator it = arrows.begin();
    bool removed = false;

    while(it != arrows.end()) {
        if(compare(*it)) { it = arrows.erase(it); removed = true; } 
        else ++it;
    }
    return removed;
}

bool Arrow::compare(Arrow& arrow){
    return arrow.getFilename() == m_filename && arrow.getRotation() == m_rotation;
}
