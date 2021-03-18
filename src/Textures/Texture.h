#pragma once
#include <glm/glm.hpp>

class Texture {
public:
    Texture() {}
    virtual ~Texture() = 0 {}

    virtual glm::vec3 GetTexel(glm::vec2 uv) const = 0;
};