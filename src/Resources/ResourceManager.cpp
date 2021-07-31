//
// Created by jeditux on 6/4/21.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string &executablePath) {
    size_t lastSlashPos = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, lastSlashPos);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShader(const std::string &shaderName,
                                                                     const std::string &vertexPath,
                                                                     const std::string &fragmentPath) {
    std::string vertexShaderSource = getFileData(vertexPath);
    if (vertexShaderSource.empty()) {
        std::cerr << "No vertex shader!" << std::endl;
        return nullptr;
    }
    std::string fragmentShaderSource = getFileData(fragmentPath);
    if (fragmentShaderSource.empty()) {
        std::cerr << "No fragment shader!" << std::endl;
        return nullptr;
    }
    auto [it, is_new] = m_shaderPrograms.try_emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexShaderSource, fragmentShaderSource));
    if (!it->second->isCompiled()) {
        std::cerr << "Can't load shader program!\n"
                  << "Vertex shader path: " << vertexPath
                  << "\nFragment shader path: " << fragmentPath << std::endl;
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string &shaderName) const {
    if (m_shaderPrograms.count(shaderName) == 0) {
        std::cerr << "Can't find shader program: " << shaderName << std::endl;
        return nullptr;
    }
    return m_shaderPrograms.at(shaderName);
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) {
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((m_path + "/" + RESOURCES_PATH + TEXTURES_PATH + texturePath).c_str(), &width, &height, &nChannels, 0);
    if (!data) {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }
    auto [it, is_new] = m_textures.try_emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, data, nChannels, GL_NEAREST, GL_CLAMP_TO_EDGE));
    stbi_image_free(data);
    return it->second;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string &textureName) const {
    if (m_textures.count(textureName) == 0) {
        std::cerr << "Can't find texture: " << textureName << std::endl;
        return nullptr;
    }
    return m_textures.at(textureName);
}

std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
                                                              const std::string& textureName,
                                                              const std::string& shaderName,
                                                              const unsigned int spriteWidth,
                                                              const unsigned int spriteHeight,
                                                              const std::string& subTextureName) {
    auto pTexture = getTexture(textureName);
    if (!pTexture) {
        std::cerr << "Can't find texture: " << textureName << " for sprite " << spriteName << std::endl;
        return nullptr;
    }
    auto pShader = getShader(shaderName);
    if (!pShader) {
        std::cerr << "Can't find shader: " << shaderName << " for sprite " << spriteName << std::endl;
        return nullptr;
    }
    auto [it, is_new] = m_sprites.try_emplace(textureName, std::make_shared<Renderer::Sprite>(pTexture, subTextureName, pShader
                                                                                              , glm::vec2(0.f, 0.f)
                                                                                              , glm::vec2(spriteWidth, spriteHeight)
                                                                                              , 0.f));
    return it->second;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string &spriteName) const {
    if (m_sprites.count(spriteName) == 0) {
        std::cerr << "Can't find sprite: " << spriteName << std::endl;
        return nullptr;
    }
    return m_sprites.at(spriteName);
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,
                                                      std::string texturePath,
                                                      const std::vector<std::string>& subTextures,
                                                      const unsigned int subTextureWidth,
                                                      const unsigned int subTextureHeight) {
    auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
    if (pTexture) {
        const unsigned int textureWidth = pTexture->width();
        const unsigned int textureHeight = pTexture->height();
        unsigned int currentTextureOffsetX = 0, currentTextureOffsetY = textureHeight;
        for (const auto& subTextureName : subTextures) {
            glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
                                   static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
            glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
                                   static_cast<float>(currentTextureOffsetY) / textureHeight);
            pTexture->addSubTexture(subTextureName, leftBottomUV, rightTopUV);
            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth) {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
    return pTexture;
}

std::string ResourceManager::getFileData(const std::string &fileName) const {
    std::ifstream vertexInput(m_path + "/" + RESOURCES_PATH + SHADERS_PATH + fileName);
    if (!vertexInput.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << vertexInput.rdbuf();
    return buffer.str();
}
