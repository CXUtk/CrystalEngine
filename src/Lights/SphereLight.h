#pragma once
#include "Light.h"
class SphereLight : public Light {
public:
    SphereLight(glm::vec3 pos, float r, glm::vec3 color, float power);
    ~SphereLight() override;
    glm::vec3 SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const override;
    float GetArea() const override;

private:
    glm::vec3 _pos, _color;
    float _power, _radius;
    float _surfaceArea;
};