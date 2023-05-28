#pragma once
#include "../Pieces/Piece.hpp"

#include <vector>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

typedef std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureMap;
typedef std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> audioMap;
typedef std::unordered_map<std::string, std::shared_ptr<sf::Font>> fontsMap;


class RessourceManager
{
public:
    RessourceManager() = delete;

    static sf::SoundBuffer getSound();
    static void setSounds();

    static void loadRessources();

    static void addTexture(const std::string& name);
    static void addFont(const std::string& name);
    static std::shared_ptr<sf::Texture> getTexture(const std::string &name);
    static std::shared_ptr<sf::Texture> getTexture(const std::shared_ptr<Piece>& piece) {
        return getTexture(piece->getFileName());
    }
    static std::shared_ptr<sf::Font> getFont(const std::string &name);

    static std::string getIconPath(const std::string& filename) { return iconsPath + filename; }
    static std::string getAudioPath(const std::string& filename) { return audioPath + filename; }
    static std::string getFontPath(const std::string& filename) { return fontPath + filename; }

private:
    inline const static std::string iconsPath = "./assets/icons/";
    inline const static std::string audioPath = "./assets/sounds/";
    inline const static std::string fontPath = "./assets/fonts/";

    inline static constexpr uint8_t m_numberOfTextures = 36;
    inline const static std::string m_textureNames[m_numberOfTextures] = {
        "circle.png", "empty_circle.png",
        "bb.png", "bw.png", "kb.png", "kw.png", "nb.png", "nw.png", "pb.png", "pw.png",
        "qb.png", "qw.png", "rb.png", "reset.png", "resetWhite.png", "rw.png", "dropDown.png",
        "dropDownWhite.png", "flip.png", "flipWhite.png",
        "arrow_n1x.png", "arrow_n2x.png", "arrow_n3x.png", "arrow_n4x.png", "arrow_n5x.png",
        "arrow_n6x.png", "arrow_n7x.png", "arrow_Nur.png", "arrow_Nru.png", "arrow_d1x.png",
        "arrow_d2x.png","arrow_d3x.png","arrow_d4x.png","arrow_d5x.png","arrow_d6x.png",
        "arrow_d7x.png"
    };

    inline const static std::string m_fontNames[1] = {"Arial.ttf"};
    inline static textureMap m_textures;
    inline static fontsMap m_fonts;
};
