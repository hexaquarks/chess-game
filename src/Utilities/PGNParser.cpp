#include "../../include/Utilities/PGNParser.hpp"
#include "../../include/Utilities/Move.hpp"
#include <fstream>


void addMove(std::vector<std::string>& movesString, const std::string& lastMove) {
    if (
        !lastMove.empty() && lastMove != "1/2-1/2" &&
        lastMove != "*" && lastMove != "1-0" &&
        lastMove != "0-1" && lastMove != "0-0" &&
        lastMove != "0-1/2" && lastMove != "1/2-0"
    ) {
        movesString.push_back(lastMove);
    }
}

PGNParser::PGNParser() {}

void PGNParser::loadFromFile(const char* fileName) {
    moves.clear();
    std::string line;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return;
    }

    while (getline(file, line)) {
        bool ignore = false;
        std::string lastMove;
        for (char c: line) {
            if (c == '}' || c == ']') {
                ignore = false;
                continue;
            }

            if (ignore) {
                continue;
            }

            switch (c) {
                case ' ':
                    addMove(moves, lastMove);
                    lastMove.clear();
                    break;
                case '.':
                    lastMove.clear();
                    break;
                case '{': case '[':
                    addMove(moves, lastMove);
                    ignore = true;
                    break;
                default:
                    lastMove += c;
            }
        }
        addMove(moves, lastMove);
    }

    file.close();
}

void PGNParser::loadFromFile(const std::string& fileName) {
    loadFromFile(fileName.c_str());
}
