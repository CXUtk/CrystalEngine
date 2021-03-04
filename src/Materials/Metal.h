#pragma once
#include "Material.h"

class Metal : public Material {
public:
    Metal(glm::vec3 color, float smoothness);
    ~Metal() override;

    glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) override;
    glm::vec3 SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf) override;
    glm::vec3 Merge(const HitRecord& hitRecord, const glm::vec3& wOut, glm::vec3 Ldir, glm::vec3 Lindir) override;
private:
    glm::vec3 _color;
    float _smoothness;
};
