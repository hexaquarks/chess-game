#pragma once
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
typedef vector<string, unique_ptr<sf::Texture>> textureVector;
typedef vector<string, unique_ptr<sf::SoundBuffer>> audioVector;

class RessourceManager {  
    inline const static string iconsPath = "../assets/icons/";
    inline const static string audioPath = "../assets/sounds/";
    string m_textureNames[2] = {"circle.png", "empty_circle.png"};

    audioVector m_audios;
    textureVector m_textures;

    public:
    RessourceManager(); // Constructor
    ~RessourceManager(); // destructor

    sf::SoundBuffer getSound();  
    void setSounds();

    void addTexture(const string& name);
    unique_ptr<sf::Texture> getTexture(const string &name);

    static string getIconPath(const string& filename) { return iconsPath + filename; }
    static string getAudioPath(const string& filename) { return audioPath + filename; }
};
