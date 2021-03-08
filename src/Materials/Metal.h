#pragma once
#include "Material.h"

class Metal : public Material {
public:
    Metal(glm::vec3 color, float smoothness);
    ~Metal() override;

    //glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const override;
    //bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const override;
    //glm::vec3 Merge(const SurfaceInteraction& isec, const glm::vec3& wOut, glm::vec3 Ldir, glm::vec3 Lindir) override;
    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;
private:
    glm::vec3 _color;
    float _smoothness;
};
