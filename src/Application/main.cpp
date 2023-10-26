#include "../../include/Application/GameThread.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
    game::GameThread gameThread;
    gameThread.startGame();
    return 0;
}
