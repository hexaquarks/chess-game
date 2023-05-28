#pragma once

#include "UIConstants.hpp"

#include <string>
#include <vector>
#include <utility>

typedef std::pair<int, int> coor2d;

class Arrow
{
public:
    Arrow(): m_rotation(0), m_size(0){};
    Arrow(coor2d, coor2d, int, std::string);

    std::string getFilename() { return m_filename; }
    coor2d getOrigin() { return m_origin; }
    coor2d getFormattedOrigin() const;
    coor2d getDestination() { return m_destination; }
    int getRotation() { return m_rotation; }

    void setCoordinates(const coor2d&,const coor2d&);
    void setDestination(const coor2d&);
    void setOrigin(const coor2d&);
    void updateArrow();
    void resetParameters();
    bool removeArrow(std::vector<Arrow>&) const;
    bool isDrawable() const;
    bool isLArrow() { return m_isLArrow; }

private:
    coor2d m_origin; // Origin absolute coordinate
    coor2d m_destination; // Destination absolute coordinate
    std::string m_filename; // File name for the arrow
    int m_dx, m_dy; // Tile differential coordinates
    int m_rotation; // Multiples of 45 degrees
    int m_size; // Size of the arrow (0 to 7)
    bool m_isLArrow = false;

    bool operator==(Arrow&) const;
};
