#include "iostream"
#include "vector"


class Move;

class PGNParser
{
    std::vector<std::string> moves;
    void split(const char* data);

    public:
    PGNParser();
    void loadFromFile(const char* fileName);
    void loadFromFile(const std::string& fileName);
};
