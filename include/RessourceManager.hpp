#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

using namespace std;

typedef unordered_map<string, shared_ptr<sf::Texture>> textureMap;
typedef unordered_map<string, shared_ptr<sf::SoundBuffer>> audioMap;

class RessourceManager {  
    inline const static string iconsPath = "../assets/icons/";
    inline const static string audioPath = "../assets/sounds/";
    string m_textureNames[2] = {"circle.png", "empty_circle.png"};

    // audioVector m_audios;
    textureMap m_textures;

    public:
    RessourceManager(); // Constructor
    // ~RessourceManager(); // destructor

    sf::SoundBuffer getSound();  
    void setSounds();

    void loadRessources();

    void addTexture(const string& name);
    shared_ptr<sf::Texture> getTexture(const string &name);

    static string getIconPath(const string& filename) { return iconsPath + filename; }
    static string getAudioPath(const string& filename) { return audioPath + filename; }
};
