#pragma once
#include "Material.h"

class Phong_Blinn : public Material {
public:
    Phong_Blinn(glm::vec3 baseColor, float expK, glm::vec3 coeff);
    ~Phong_Blinn() override;

    glm::vec3 BSDF(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) override;
    glm::vec3 Le(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut) override;
private:
    glm::vec3 _baseColor;
    float _expK;
    glm::vec3 _coeff;
};
