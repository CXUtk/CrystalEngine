#pragma once
#include "Material.h"

class Phong : public Material {
public:
    Phong(glm::vec3 baseColor, float expK, glm::vec3 coeff);
    ~Phong() override;

    glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const override;
    bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const override;
    glm::vec3 Le(const SurfaceInteraction& isec, glm::vec3 wOut) override;
    void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;
private:
    glm::vec3 _baseColor;
    float _expK;
    glm::vec3 _coeff;
};
