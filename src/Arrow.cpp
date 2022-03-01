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
    m_origin.first = (origin.first / CELL_SIZE) * CELL_SIZE + CELL_SIZE/2;
    m_origin.second = (origin.second / CELL_SIZE) * CELL_SIZE + MENUBAR_HEIGHT + CELL_SIZE/2;
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
    if((m_dx == 0 && m_dy == 0)) return;
    if (m_dx != 0 || m_dy != 0) m_rotation = rotation[1 + sign(m_dx)][1 + sign(m_dy)];
    
    int size = std::max(abs(m_dx), abs(m_dy));
    if(size == 0) return; // do nothing, arrow too short
    m_filename = "arrow_n" + to_string(size) + "x.png";
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
        if(compare(*it)) { it = arrows.erase(it); removed = true;} 
        else ++it;
    }
    return removed;
}

bool Arrow::compare(Arrow& arrow){
    return arrow.getOrigin() == m_origin && arrow.getFilename() == m_filename && 
    arrow.getRotation() == m_rotation;
}
