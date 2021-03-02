#pragma once
#include "Material.h"

class Strauss : public Material {
public:
    Strauss(glm::vec3 baseColor, float smoothness, float metalness, float transparency);
    ~Strauss() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
    glm::vec3 Le(const HitRecord& hitRecord, glm::vec3 wOut) override;
private:
    glm::vec3 _baseColor;
    float _smoothness, _metalness, _transparency;
};
