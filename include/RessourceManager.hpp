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
    inline static constexpr uint8_t m_numberOfTextures = 18;
    string m_textureNames[m_numberOfTextures] = {"circle.png", "empty_circle.png", 
        "bb.png", "bw.png", "kb.png", "kw.png", "nb.png", "nw.png", "pb.png", "pw.png",
        "qb.png", "qw.png", "rb.png", "reset.png", "rw.png", "dropDown.png", "flip.png",
        "arrow_n2x.png"};

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
