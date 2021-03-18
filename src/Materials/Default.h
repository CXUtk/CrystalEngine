#pragma once
#include "Material.h"

class Default : public Material {
public:
    Default(glm::vec3 color);
    Default(glm::vec3 color, glm::vec2 uvExt);
    Default(glm::vec3 color, glm::vec3 color2, glm::vec2 uvExt);
    ~Default() override;

    //glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const override;
    //bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const override;
    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _color, _color2;
    glm::vec2 _uvExtend;
};
