#pragma once
#include "Material.h"

class Default : public Material {
public:
    Default(glm::vec3 color);
    Default(glm::vec3 color, glm::vec2 uvExt);
    ~Default() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
    bool SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf, glm::vec3& dir) override;

private:
    glm::vec3 _color;
    glm::vec2 _uvExtend;
};
