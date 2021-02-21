#pragma once
#include "Material.h"

class Default : public Material {
public:
    Default(glm::vec3 color);
    ~Default() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
private:
    glm::vec3 _color;
};