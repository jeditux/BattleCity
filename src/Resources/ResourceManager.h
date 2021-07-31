//
// Created by jeditux on 6/4/21.
//

#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class ShaderProgram;
    class Texture2D;
    class Sprite;
}

class ResourceManager {
public:
    ResourceManager(const std::string& executablePath);
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&&) = delete;

    std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName) const;

    std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
    std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName) const;

    std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
                                                const std::string& textureName,
                                                const std::string& shaderName,
                                                const unsigned int spriteWidth,
                                                const unsigned int spriteHeight,
                                                 const std::string& subTextureName = "default");
    std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName) const;

    std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,
                                                          std::string texturePath,
                                                          const std::vector<std::string>& subTextures,
                                                          const unsigned int subTextureWidth,
                                                          const unsigned int subTextureHeight);

private:
    const std::string RESOURCES_PATH = "res/";
    const std::string SHADERS_PATH = "shaders/";
    const std::string TEXTURES_PATH = "textures/";

    std::string m_path;

    typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap m_shaderPrograms;

    typedef std::map<std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
    TexturesMap m_textures;

    typedef std::map<std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
    SpritesMap m_sprites;

    std::string getFileData(const std::string& fileName) const;

};


