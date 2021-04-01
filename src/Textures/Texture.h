#pragma once
#include <glm/glm.hpp>

enum class TextureFormat {
    RGBA32,
    RGB24,
    DEPTH32,
    GRAYSCALE8
};

class Texture {
public:
    Texture() {}
    virtual ~Texture() = 0 {}

    virtual glm::vec3 GetTexel(glm::vec2 uv) const = 0;
};
