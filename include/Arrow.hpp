#include <vector>
#include <utility>


typedef std::pair<int, int> coor2d;

class Arrow {
    coor2d m_origin;
    coor2d m_destination;
    // string m_filename;
    int m_size;

    public:
    Arrow(coor2d&, coor2d&);
    coor2d getOrigin() { return m_origin; }

};