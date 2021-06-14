//
// Created by jeditux on 6/14/21.
//

#pragma once

#include <glad/glad.h>

namespace Renderer {

    class Texture2D {
    public:
        Texture2D(int width, int height, const unsigned char* data,
                  unsigned int nChannels = 4, GLenum filter = GL_LINEAR, GLenum wrapMode = GL_CLAMP_TO_EDGE);

        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D(Texture2D&&);
        Texture2D& operator=(Texture2D&&);
        ~Texture2D();

        void bind() const;

    private:
        GLuint m_Id;
        GLenum m_mode;
        int m_width;
        int m_height;
    };

}


