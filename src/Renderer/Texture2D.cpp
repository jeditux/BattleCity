//
// Created by jeditux on 6/14/21.
//

#include "Texture2D.h"

namespace Renderer {
    Texture2D::Texture2D(int width, int height, const unsigned char* data, unsigned int nChannels,
                         GLenum filter, const GLenum wrapMode) : m_width(width), m_height(height) {
        switch (nChannels) {
            case 4:
                m_mode = GL_RGBA;
                break;
            case 3:
                m_mode = GL_RGB;
                break;
            default:
                m_mode = GL_RGBA;
                break;
        }
        glGenTextures(1, &m_Id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture2D::Texture2D(Texture2D&& texture2D) {
        m_Id = texture2D.m_Id;
        texture2D.m_Id = 0;
        m_mode = texture2D.m_mode;
        m_width = texture2D.m_width;
        m_height = texture2D.m_height;
    }

    Texture2D& Texture2D::operator=(Texture2D&& texture2D) {
        glDeleteTextures(1, &m_Id);
        m_Id = texture2D.m_Id;
        texture2D.m_Id = 0;
        m_mode = texture2D.m_mode;
        m_width = texture2D.m_width;
        m_height = texture2D.m_height;
        return *this;
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_Id);
    }

    void Texture2D::bind() const {
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    void Texture2D::addSubTexture(const std::string &name, const glm::vec2 &leftBottomUV,
                                  const glm::vec2 &rightTopUV) {
        m_subTextures.emplace(name, SubTexture2D(leftBottomUV, rightTopUV));
    }

    const Texture2D::SubTexture2D & Texture2D::getSubTexture(const std::string &name) const {
        auto it = m_subTextures.find(name);
        if (it == m_subTextures.end()) {
            const static SubTexture2D defaultSubTexture;
            return defaultSubTexture;
        }
        return it->second;
    }
}
