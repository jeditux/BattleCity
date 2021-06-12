//
// Created by jeditux on 6/4/21.
//

#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class ShaderProgram;
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

private:
    const std::string RESOURCES_PATH = "res";
    const std::string SHADERS_PATH = "res/shaders/";

    std::string m_path;

    typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap m_shaderPrograms;

    std::string getFileData(const std::string& fileName) const;

};


