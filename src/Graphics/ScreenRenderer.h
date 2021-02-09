#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
class ScreenRenderer {
public:
    ScreenRenderer();
    void draw();
    void copyTexture(const unsigned char* data, int width, int height);

private:
    GLuint _textureID;
    GLuint _vao, _vbo;
};
