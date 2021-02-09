#pragma once
#include "Material.h"

class Default : public Material {
public:
    Default(glm::vec3 color);
    ~Default() override;

    glm::vec3 Shade() override;
private:
    glm::vec3 _color;
};