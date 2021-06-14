//
// Created by jeditux on 6/4/21.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
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