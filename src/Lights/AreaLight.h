#pragma once
#include "Light.h"
class AreaLight : public Light {
public:
    AreaLight(glm::vec3 pos, glm::vec3 u, glm::vec3 v, float power);
    ~AreaLight() override;
    glm::vec3 SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const override;
    float GetArea() const override { return glm::length(glm::cross(_u, _v)); }
    glm::vec3 IntensityPerArea() const override { return glm::vec3(_power); }
    glm::vec3 SampleEmission(glm::vec3* pos, glm::vec3* dir, float* pdf) const override;

private:
    glm::vec3 _pos, _u, _v;
    float _power;
};
