#pragma once

#include <string>
#include <vector>
#include <utility>
using namespace std;

typedef std::pair<int, int> coor2d;

inline constexpr int rotation[3][3] = {
        {225, 180, 135},
        {270,   0,  90},
        {315,   0,  45},
    };
class Arrow {
    coor2d m_origin;
    coor2d m_destination;
    string m_filename; 
    int m_rotation;
    int m_size;

    public:
    Arrow(): m_rotation(0), m_size(0){};
    Arrow(coor2d, coor2d, int, string);

    int getRotation() { return m_rotation; }
    string getFilename() { return m_filename; }
    
    void updateArrow();
    void setOrigin(coor2d&); 
    void resetParameters();
    void setCoordinates(coor2d&,coor2d&);
    void setDestination(coor2d& destination){ m_destination = destination; }
    coor2d getOrigin() { return m_origin; }
    coor2d getDestination() { return m_destination; }
    bool removeArrow(vector<Arrow>&);
    bool isDrawable();
    bool compare(Arrow&);
    
};