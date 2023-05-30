#include "../include/GameThread.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
    GameThread::startGame();
    return 0;
}
