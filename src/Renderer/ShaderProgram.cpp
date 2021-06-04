//
// Created by jeditux on 6/4/21.
//

#include <iostream>
#include "ShaderProgram.h"

namespace Renderer {
    ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader) {
        GLuint vertexShaderId;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderId)) {
            std::cerr << "VERTEX SHADER compile time error" << std::endl;
            return;
        }
        GLuint fragmentShaderId;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderId)) {
            std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
            glDeleteShader(vertexShaderId);
            return;
        }

        m_Id = glCreateProgram();
        glAttachShader(m_Id, vertexShaderId);
        glAttachShader(m_Id, fragmentShaderId);
        glLinkProgram(m_Id);

        GLint success;
        glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_Id, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link time error:\n" << infoLog << std::endl;
        } else {
            m_isCompiled = true;
        }
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderId) {
        shaderId = glCreateShader(shaderType);
        const char* cSource = source.c_str();
        glShaderSource(shaderId, 1, &cSource, nullptr);
        glCompileShader(shaderId);

        GLint success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_Id);
    }

    void ShaderProgram::use() const {
        glUseProgram(m_Id);
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) noexcept {
        m_Id = shaderProgram.m_Id;
        m_isCompiled = shaderProgram.m_isCompiled;
        shaderProgram.m_Id = 0;
        shaderProgram.m_isCompiled = false;
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram &&shaderProgram) noexcept {
        glDeleteProgram(m_Id);
        m_Id = shaderProgram.m_Id;
        m_isCompiled = shaderProgram.m_isCompiled;
        shaderProgram.m_Id = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }
}
