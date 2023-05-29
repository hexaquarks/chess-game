#include <utility>
#include <algorithm>
#include "../../include/Utilities/Arrow.hpp"

constexpr int g_rotation[3][3] =
{
        { 225, 180, 135 },
        { 270, 0, 90 },
        { 315, 0, 45 },
};

const std::vector<coor2d> urCoords = {{1, -2}, {2, 1}, {-1, 2}, {-2, -1}};
const std::vector<coor2d> ruCoords = {{2, -1}, {1, 2}, {-2, 1}, {-1, -2}};

template <typename T>
int sign(T val_)
{
    return (T(0) < val_) - (val_ < T(0));
}

namespace // anonymous namespace
{
    bool checkOutOfBounds(const coor2d& destination_)
    {
        return (
            destination_.first < 0 || destination_.first > ui::g_WINDOW_SIZE ||
            destination_.second < 0 || destination_.second > ui::g_WINDOW_SIZE
        );
    }

    void checkKnightSquares(int dx_, int dy_, int& rotation_, std::string& filename_, bool& isLarrow_)
    {
        auto it_ur = urCoords.begin();
        auto it_ru = ruCoords.begin();

        while (it_ur != urCoords.end())
        {
            if (it_ur->first == dx_ && it_ur->second == dy_)
            {
                rotation_ = (it_ur - urCoords.begin()) * 90;
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

Arrow::Arrow(coor2d origin_, coor2d destination_, int rotation_, std::string filename_)
: m_origin(origin_), m_destination(destination_), m_rotation(rotation_), m_filename(filename_)
{
}

void Arrow::setOrigin(const coor2d& origin_)
{
    setPoint(m_origin, origin_);
}

void Arrow::setDestination(const coor2d& destination_)
{
    setPoint(m_destination, destination_);

    int formattedCellSize = static_cast<int>(ui::g_CELL_SIZE);
    m_dx = (m_destination.first / formattedCellSize - m_origin.first / formattedCellSize);
    m_dy = (m_destination.second / formattedCellSize - m_origin.second / formattedCellSize);
}

coor2d Arrow::getFormattedOrigin() const
{
    const int x = (m_origin.first / ui::g_CELL_SIZE) * ui::g_CELL_SIZE + ui::g_CELL_SIZE/2;
    const int y = (m_origin.second / ui::g_CELL_SIZE) * ui::g_CELL_SIZE + ui::g_CELL_SIZE/2 + ui::g_MENUBAR_HEIGHT;
    return {x, y};
}

void Arrow::updateArrow()
{
    // Check if arrow is feasible
    if (m_dx == 0 && m_dy == 0) return; // Do nothing, arrow is at the same spot
    if (checkOutOfBounds(m_destination)) return; // Do nothing, arrow out of window

    if (m_dx != 0 || m_dy != 0) m_rotation = g_rotation[1 + sign(m_dx)][1 + sign(m_dy)];

    const int size = std::max(abs(m_dx), abs(m_dy));
    if (size == 0) return; // Do nothing, arrow too short
    
    bool isKnightArrow = abs(abs(m_dx)-abs(m_dy)) == 1 && abs(m_dx) > 0 && abs(m_dy) > 0;
    bool isDiagonalArrow = abs(m_dx) == abs(m_dy);

    if (isKnightArrow) 
    {
        checkKnightSquares(m_dx, m_dy,m_rotation, m_filename, m_isLArrow);
    }
    else if (isDiagonalArrow)
    {
        m_filename = "arrow_d" + std::to_string(size) + "x.png";
        m_isLArrow = false;
    }
    else
    {
        m_filename = "arrow_n" + std::to_string(size) + "x.png";
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

bool Arrow::removeArrow(std::vector<Arrow>& arrows_) const
{
    auto it = arrows_.begin();
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

void setPoint(coor2d& point_, const coor2d& value_)
{
    point_.first = value_.first;
    point_.second = value_.second - ui::g_MENUBAR_HEIGHT;
}