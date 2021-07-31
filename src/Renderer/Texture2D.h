//
// Created by jeditux on 6/14/21.
//

#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <map>
#include <string>

namespace Renderer {

    class Texture2D {
    public:

        struct SubTexture2D {
            glm::vec2 leftBottomUV;
            glm::vec2 rightTopUV;

            SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV)
                : leftBottomUV(_leftBottomUV)
                , rightTopUV(_rightTopUV)
            {}

            SubTexture2D()
                : leftBottomUV(0.f)
                , rightTopUV(1.f)
            {}
        };

        Texture2D(int width, int height, const unsigned char* data,
                  unsigned int nChannels = 4, GLenum filter = GL_LINEAR, GLenum wrapMode = GL_CLAMP_TO_EDGE);

        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D(Texture2D&&);
        Texture2D& operator=(Texture2D&&);
        ~Texture2D();

        void bind() const;
        unsigned int width() const { return m_width; }
        unsigned int height() const { return m_height; }

        void addSubTexture(const std::string& name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
        const SubTexture2D& getSubTexture(const std::string& name) const;

    private:
        GLuint m_Id;
        GLenum m_mode;
        int m_width;
        int m_height;

        std::map<std::string, SubTexture2D> m_subTextures;
    };

}


