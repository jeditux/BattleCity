#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Resources/ResourceManager.h"

glm::ivec2 windowSize(640, 480);

GLfloat vertices[] = {
        0.0f, 50.f, 0.0f,
        50.f, -50.f, 0.0f,
        -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
};

GLfloat texCoords[] = {
        0.5f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
};

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    windowSize.x = width;
    windowSize.y = height;
    glViewport(0, 0, windowSize.x, windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(windowSize.x, windowSize.y, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Can't load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 0, 1);
    {
        ResourceManager resourceManager(argv[0]);
        auto shaderProgram = resourceManager.loadShader("TestTriangle", "triangle.vert", "triangle.frag");
        if (!shaderProgram) {
            std::cerr << "Can't create shader program!" << std::endl;
            return -1;
        }

        auto spriteShaderProgram = resourceManager.loadShader("SpriteShader", "sprite.vert", "sprite.frag");
        if (!spriteShaderProgram) {
            std::cerr << "Can't create sprite shader program!" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "map_16x16.png");

        auto pSprite = resourceManager.loadSprite("testSprite", "DefaultTexture", "SpriteShader", 50, 100);
        pSprite->setPosition(glm::vec2(300, 100));

        GLuint pointsVBO;
        glGenBuffers(1, &pointsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint colorsVBO;
        glGenBuffers(1, &colorsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoordsVBO;
        glGenBuffers(1, &texCoordsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        shaderProgram->use();
        shaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix1 = glm::mat4(1.f);
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(100.f, 50.f, 0.f));

        glm::mat4 modelMatrix2 = glm::mat4(1.f);
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(590.f, 50.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(windowSize.x), 0.f, static_cast<float>(windowSize.y), -100.f, 100.f);

        shaderProgram->setMatrix4("projectionMatrix", projectionMatrix);

        spriteShaderProgram->use();
        spriteShaderProgram->setInt("tex", 0);
        spriteShaderProgram->setMatrix4("projectionMatrix", projectionMatrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            shaderProgram->setMatrix4("modelMatrix", modelMatrix1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            shaderProgram->setMatrix4("modelMatrix", modelMatrix2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->draw();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}
