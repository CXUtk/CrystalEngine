#pragma once
#include "Material.h"

class Phong : public Material {
public:
    Phong(glm::vec3 baseColor, float expK, glm::vec3 coeff);
    ~Phong() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
    bool SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf, glm::vec3& dir) override;
    glm::vec3 Le(const HitRecord& hitRecord, glm::vec3 wOut) override;
private:
    glm::vec3 _baseColor;
    float _expK;
    glm::vec3 _coeff;
};
