#pragma once
#include "../Logic/Pieces/Piece.hpp"

#include <vector>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

template<typename T>
using ResourceMap = std::unordered_map<std::string, std::shared_ptr<T>>;

using TextureMap = ResourceMap<sf::Texture>;
using AudioMap = ResourceMap<sf::SoundBuffer>;
using FontsMap = ResourceMap<sf::Font>;

class RessourceManager
{
public:
    RessourceManager() = delete;

    static sf::SoundBuffer getSound();
    static void setSounds();

    static void loadRessources();

    template<typename Resource, typename Container>
    static void addResource(const std::string&, const std::string&, Container&);

    template<typename Resource>
    static std::shared_ptr<Resource> getResource(const std::string&, const ResourceMap<Resource>&);

    static std::shared_ptr<sf::Texture> getTexture(const std::string&);
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
    inline static TextureMap m_textures;
    inline static FontsMap m_fonts;
};

template<typename Resource, typename Container>
void RessourceManager::addResource(const std::string& resourceName_, const std::string& filepath_, Container& resources_)
{
    auto resource = std::make_shared<Resource>();
    resource->loadFromFile(filepath_);
    resources_.emplace(resourceName_, resource);
}

template<typename Resource>
std::shared_ptr<Resource> RessourceManager::getResource(const std::string& name_, const ResourceMap<Resource>& map_) {
    for (const auto& [first, second] : map_)
    {
        if (first == name_) return second;
    }
    return {};
}