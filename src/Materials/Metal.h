#pragma once
#include "Material.h"

class Metal : public Material {
public:
    Metal(glm::vec3 color);
    ~Metal() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
private:
    glm::vec3 _color;
};
