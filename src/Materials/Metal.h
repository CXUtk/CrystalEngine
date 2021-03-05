#pragma once
#include "Material.h"

class Metal : public Material {
public:
    Metal(glm::vec3 color, float smoothness);
    ~Metal() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) override;
    bool SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf, glm::vec3& dir) override;
    glm::vec3 Merge(const HitRecord& hitRecord, const glm::vec3& wOut, glm::vec3 Ldir, glm::vec3 Lindir) override;
private:
    glm::vec3 _color;
    float _smoothness;
};
