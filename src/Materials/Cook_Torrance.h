#pragma once
#include "Material.h"

class Cook_Torrance : public Material {
public:
    Cook_Torrance(glm::vec3 baseColor, float roughness);
    ~Cook_Torrance() override;

    glm::vec3 Li(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
    glm::vec3 Le(const HitRecord& hitRecord, glm::vec3 wOut) override;
private:
    glm::vec3 _baseColor;
    float _roughness;
};
