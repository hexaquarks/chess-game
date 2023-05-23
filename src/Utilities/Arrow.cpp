#include <utility>
#include <algorithm>
#include "../../include/Utilities/Arrow.hpp"

constexpr int g_rotation[3][3] =
{
        { 225, 180, 135 },
        { 270, 0, 90 },
        { 315, 0, 45 },
};

const vector<coor2d> urCoords = {{1, -2}, {2, 1}, {-1, 2}, {-2, -1}};
const vector<coor2d> ruCoords = {{2, -1}, {1, 2}, {-2, 1}, {-1, -2}};

template <typename T>
int sign(T val_)
{
    return (T(0) < val_) - (val_ < T(0));
}

namespace // anonymous namespace
{
    bool checkOutOfBounds(coor2d& destination_)
    {
        return (
            destination_.first < 0 || destination_.first > ui::g_WINDOW_SIZE ||
            destination_.second < 0 || destination_.second > ui::g_WINDOW_SIZE
        );
    }

    void checkKnightSquares(int dx_, int dy_, int& rotation_, std::string& filename_, bool& isLarrow_)
    {
        vector<coor2d>::const_iterator it_ur = urCoords.begin();
        vector<coor2d>::const_iterator it_ru = ruCoords.begin();

        while (it_ur != urCoords.end())
        {
            if (it_ur->first == dx_ && it_ur->second == dy_)
            {
                rotation_ = (it_ur - urCoords.begin())*90;
                filename_ = "arrow_Nur.png";
                break;
            }
            if (it_ru->first == dx_ && it_ru->second == dy_)
            {
                rotation_ = (it_ru - ruCoords.begin()) * 90;
                filename_ = "arrow_Nru.png";
                break;
            }
            ++it_ru; ++it_ur;
        }
        isLarrow_ = true;
    }
} 

Arrow::Arrow(coor2d origin_, coor2d destination_, int rotation_, string filename_)
: m_origin(origin_), m_destination(destination_), m_rotation(rotation_), m_filename(filename_)
{
}

void Arrow::setOrigin(const coor2d& origin_)
{
    // Need to format the origin to be at the center of the tile
    m_origin.first = origin_.first;
    m_origin.second = origin_.second - ui::g_MENUBAR_HEIGHT;
}

coor2d Arrow::getFormattedOrigin() const
{
    const int x = (m_origin.first / ui::g_CELL_SIZE) * ui::g_CELL_SIZE + ui::g_CELL_SIZE/2;
    const int y = (m_origin.second / ui::g_CELL_SIZE) * ui::g_CELL_SIZE + ui::g_CELL_SIZE/2 + ui::g_MENUBAR_HEIGHT;
    return {x, y};
}

void Arrow::setDestination(const coor2d& destination_)
{
    m_destination.first = destination_.first;
    m_destination.second = destination_.second - ui::g_MENUBAR_HEIGHT;

    // Tile coordinates
    m_dx = (m_destination.first/(int)ui::g_CELL_SIZE - m_origin.first/(int)ui::g_CELL_SIZE);
    m_dy = (m_destination.second/(int)ui::g_CELL_SIZE - m_origin.second/(int)ui::g_CELL_SIZE);
}

void Arrow::updateArrow()
{
    // Check if arrow is feasible
    if (m_dx == 0 && m_dy == 0) return;
    if (m_dx != 0 || m_dy != 0) m_rotation = g_rotation[1 + sign(m_dx)][1 + sign(m_dy)];

    const int size = std::max(abs(m_dx), abs(m_dy));
    if (size == 0) return; // Do nothing, arrow too short
    if (checkOutOfBounds(m_destination)) return; // Do nothing, arrow out of window

    if (abs(abs(m_dx)-abs(m_dy)) == 1 && abs(m_dx) > 0 && abs(m_dy) > 0) 
    {
        checkKnightSquares(m_dx, m_dy,m_rotation, m_filename, m_isLArrow);
    }
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
        if (*this==(*it))
        {
            it = arrows_.erase(it);
            removed = true;
        }
        else ++it;
    }
    return removed;
}

bool Arrow::operator==(Arrow& rhs_) const
{
    return (
        rhs_.getFormattedOrigin() == getFormattedOrigin() &&
        rhs_.getFilename() == m_filename &&
        rhs_.getRotation() == m_rotation
    );
}
