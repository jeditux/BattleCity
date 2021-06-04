//
// Created by jeditux on 6/4/21.
//

#ifndef BATTLECITY_SHADERPROGRAM_H
#define BATTLECITY_SHADERPROGRAM_H

#include <string>
#include <glad/glad.h>

namespace Renderer {
    class ShaderProgram {
    public:
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        ~ShaderProgram();
        bool isCompiled() const { return m_isCompiled; }
        void use() const;

        ShaderProgram(ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

    private:
        bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderId);
        bool m_isCompiled = false;
        GLuint m_Id = 0;
    };
}


#endif //BATTLECITY_SHADERPROGRAM_H
