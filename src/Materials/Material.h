#pragma once
#include <glm/glm.hpp>

class Material {
public:
    Material() {}
    virtual ~Material() = 0 {};

    virtual glm::vec3 Shade() = 0;
};
