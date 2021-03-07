#pragma once
#include "Light.h"
class PointLight : public Light {
public:
    PointLight(glm::vec3 pos, glm::vec3 color, float power);
    ~PointLight() override;
    glm::vec3 SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float& pdf) override;

private:
    glm::vec3 _pos, _color;
    float _power;
};