#include "iostream"
#include "vector"

#include <stack>

class Move;
class Board;
class MoveTreeManager;

class PGNParser
{
public:
    PGNParser() = default;
    explicit PGNParser(MoveTreeManager& moveTreeManager);

    void loadFromFile(const char* fileName);
    void loadFromFile(const std::string& fileName);

    void initializeMoveSequenceFromPNG(const std::string&);

private:
    std::vector<std::string> moves;
    MoveTreeManager& m_moveTreeManager;

    void split(const char* data);

    std::vector<std::string> tokenizePGN(const std::string& pgn);
    void parseAllTokens(const std::vector<std::string>& tokens, size_t& index, int& moveCount, std::stack<int>& undoStack);
    void addMoveToPGNTree(const std::string& token_);
};
