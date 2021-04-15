#pragma once
#include "Light.h"
#include <glm/gtx/transform.hpp>
class SphereLight : public Light {
public:
    SphereLight(glm::vec3 pos, float r, glm::vec3 color, float power);
    ~SphereLight() override;
    glm::vec3 SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const override;
    float GetArea() const override;
    glm::vec3 SampleRadiance(glm::vec3 dir) const override { return _color * _power / (4 * glm::pi<float>() * _radius * _radius); }

    glm::vec3 SampleEmission(glm::vec3* pos, glm::vec3* dir, float* pdf) const override;

private:
    glm::vec3 _pos, _color;
    float _power, _radius;
    float _surfaceArea;
};
