#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;


int main() {
    RenderWindow window(VideoMode(640, 640), "SFML works!");
    Event event;

    int* textures = new int[10];
    Texture t1; 
    t1.loadFromFile("assets/bw.png");

    
    while (window.isOpen()) {
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        std::cout << "so" << std::endl;
        Sprite WhiteBishop(t1);
        WhiteBishop.setScale(0.6,0.6);
        int i = 0; int j = 0;

        for(int i = 0 ; i < 640 ; i+=80) {
            for(int j = 0 ; j < 640 ; j+=80) {
                sf::RectangleShape square(sf::Vector2f(80,80));
                if((i + j) % 2 == 0) {
                    square.setFillColor(sf::Color(181,136,99)); 
                    square.setOutlineColor(square.getFillColor());
                } else {
                    square.setFillColor(sf::Color(240,217,181)); 
                    square.setOutlineColor(square.getFillColor());                    
                }
                square.setPosition(i,j);
                window.draw(square);

                WhiteBishop.setPosition(i,j); 
                window.draw(WhiteBishop);
                j++;
            }    
            i++; j =0;
        }
        window.display();
    }

    return 0;
}