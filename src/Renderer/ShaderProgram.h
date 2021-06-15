//
// Created by jeditux on 6/4/21.
//

#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

namespace Renderer {
    class ShaderProgram {
    public:
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        ~ShaderProgram();
        bool isCompiled() const { return m_isCompiled; }
        void use() const;
        void setInt(const std::string& name, const GLint value);
        void setMatrix4(const std::string& name, const glm::mat4& matrix);

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


