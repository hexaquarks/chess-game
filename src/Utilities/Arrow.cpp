#include <utility>
#include <algorithm>
#include "../../include/Utilities/Arrow.hpp"
#include "../../include/GameThread.hpp"

template <typename T> int sign(T val_)
{
    return (T(0) < val_) - (val_ < T(0));
}

Arrow::Arrow(coor2d origin_, coor2d destination_, int rotation_, string filename_)
: m_origin(origin_), m_destination(destination_), m_rotation(rotation_), m_filename(filename_)
{
}

void Arrow::setOrigin(const coor2d& origin_)
{
    // Need to format the origin to be at the center of the tile
    m_origin.first = origin_.first;
    m_origin.second = origin_.second - g_MENUBAR_HEIGHT;
}

coor2d Arrow::getFormattedOrigin() const
{
    int x = (m_origin.first / g_CELL_SIZE) * g_CELL_SIZE + g_CELL_SIZE/2;
    int y = (m_origin.second / g_CELL_SIZE) * g_CELL_SIZE + g_CELL_SIZE/2 + g_MENUBAR_HEIGHT;
    return {x, y};
}

void Arrow::setDestination(const coor2d& destination_)
{
    m_destination.first = destination_.first;
    m_destination.second = destination_.second - g_MENUBAR_HEIGHT;

    // Tile coordinates
    m_dx = (m_destination.first/(int)g_CELL_SIZE - m_origin.first/(int)g_CELL_SIZE);
    m_dy = (m_destination.second/(int)g_CELL_SIZE - m_origin.second/(int)g_CELL_SIZE);
}

void Arrow::updateArrow()
{
    // Check if arrow is feasible
    if (m_dx == 0 && m_dy == 0) return;
    if (m_dx != 0 || m_dy != 0) m_rotation = g_rotation[1 + sign(m_dx)][1 + sign(m_dy)];

    int size = std::max(abs(m_dx), abs(m_dy));
    if (size == 0) return; // Do nothing, arrow too short
    if (checkOutOfBounds()) return; // Do nothing, arrow out of window

    if (abs(abs(m_dx)-abs(m_dy)) == 1 && abs(m_dx) > 0 && abs(m_dy) > 0) checkKnightSquares();
    else if (abs(m_dx) == abs(m_dy))
    {
        m_filename = "arrow_d" + to_string(size) + "x.png";
        m_isLArrow = false;
    }
    else
    {
        m_filename = "arrow_n" + to_string(size) + "x.png";
        m_isLArrow = false;
    }
}

bool Arrow::checkOutOfBounds() const
{
    return (
        m_destination.first < 0 || m_destination.first > g_WINDOW_SIZE ||
        m_destination.second < 0 || m_destination.second > g_WINDOW_SIZE
    );
}

void Arrow::checkKnightSquares()
{
    vector<coor2d>::const_iterator it_ur = urCoords.begin();
    vector<coor2d>::const_iterator it_ru = ruCoords.begin();

    while (it_ur != urCoords.end())
    {
        if (it_ur->first == m_dx && it_ur->second == m_dy)
        {
            m_rotation = (it_ur - urCoords.begin())*90;
            m_filename = "arrow_Nur.png";
            break;
        }
        if (it_ru->first == m_dx && it_ru->second == m_dy)
        {
            m_rotation = (it_ru - ruCoords.begin()) * 90;
            m_filename = "arrow_Nru.png";
            break;
        }
        ++it_ru; ++it_ur;
    }
    m_isLArrow = true;
}

void Arrow::resetParameters()
{
    m_origin = {0, 0};
    m_destination = {0, 0};
    m_dx = 0;
    m_dy = 0;
}

bool Arrow::isDrawable() const
{
    return abs(m_dx) > 0 || abs(m_dy) > 0;
}

bool Arrow::removeArrow(vector<Arrow>& arrows_) const
{
    vector<Arrow>::iterator it = arrows_.begin();
    bool removed = false;

    while (it != arrows_.end())
    {
        if (compare(*it))
        {
            it = arrows_.erase(it);
            removed = true;
        }
        else ++it;
    }
    return removed;
}

bool Arrow::compare(Arrow& arrow_) const
{
    return (
        arrow_.getFormattedOrigin() == getFormattedOrigin() &&
        arrow_.getFilename() == m_filename &&
        arrow_.getRotation() == m_rotation
    );
}
