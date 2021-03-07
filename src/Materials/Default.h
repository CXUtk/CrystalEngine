#pragma once
#include "Material.h"

class Default : public Material {
public:
    Default(glm::vec3 color);
    Default(glm::vec3 color, glm::vec2 uvExt);
    ~Default() override;

    glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const override;
    bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const override;
    void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
    glm::vec2 _uvExtend;
};
