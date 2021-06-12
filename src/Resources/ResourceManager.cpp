//
// Created by jeditux on 6/4/21.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

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
    }
    return m_shaderPrograms.at(shaderName);
}

std::string ResourceManager::getFileData(const std::string &fileName) const {
    std::ifstream vertexInput(m_path + "/" + SHADERS_PATH + fileName);
    if (!vertexInput.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << vertexInput.rdbuf();
    return buffer.str();
}
