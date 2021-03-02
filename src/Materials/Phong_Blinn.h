#pragma once
#include "Material.h"

class Phong_Blinn : public Material {
public:
    Phong_Blinn(glm::vec3 baseColor, float expK, glm::vec3 coeff);
    ~Phong_Blinn() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
    glm::vec3 Le(const HitRecord& hitRecord, glm::vec3 wOut) override;
private:
    glm::vec3 _baseColor;
    float _expK;
    glm::vec3 _coeff;
};
