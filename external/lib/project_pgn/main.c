#include "PGN.hpp"
#include <iostream>
using namespace pgnp;

int main(){
    PGN pgn;
    pgn.FromFile("example.pgn");
    pgn.ParseNextGame();

    std::cout << "Event is: " << pgn.GetTagValue("Event") << std::endl;

    HalfMove *m = new HalfMove();
    pgn.GetMoves(m);
    std::cout << "First move is: " << m->move << std::endl;
    return(0);
}