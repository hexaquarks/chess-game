#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Piece.hpp"
using namespace std;

typedef unordered_map<string, shared_ptr<sf::Texture>> textureMap;
typedef unordered_map<string, shared_ptr<sf::SoundBuffer>> audioMap;

class RessourceManager {  
    inline const static string iconsPath = "../assets/icons/";
    inline const static string audioPath = "../assets/sounds/";
    inline static constexpr uint8_t m_numberOfTextures = 26;
    inline const static string m_textureNames[m_numberOfTextures] = {"circle.png", "empty_circle.png", 
        "bb.png", "bw.png", "kb.png", "kw.png", "nb.png", "nw.png", "pb.png", "pw.png",
        "qb.png", "qw.png", "rb.png", "reset.png", "rw.png", "dropDown.png", "flip.png",
        "arrow_n1x.png", "arrow_n2x.png", "arrow_n3x.png", "arrow_n4x.png", "arrow_n5x.png",
        "arrow_n6x.png", "arrow_n7x.png", "arrow_Nur.png", "arrow_Nru.png"};

    // audioVector m_audios;
    inline static textureMap m_textures;

    public:
    RessourceManager() = delete;

    static sf::SoundBuffer getSound();  
    static void setSounds();

    static void loadRessources();

    static void addTexture(const string& name);
    static shared_ptr<sf::Texture> getTexture(const string &name);
    static shared_ptr<sf::Texture> getTexture(const Piece* piece) {
        return getTexture(piece->getFileName());
    }

    static string getIconPath(const string& filename) { return iconsPath + filename; }
    static string getAudioPath(const string& filename) { return audioPath + filename; }
};
