#pragma once
#include "Material.h"

class Brick : public Material {
public:
    Brick(glm::vec3 colorBrick, glm::vec3 colorMortor, float brickWidth, float brickHeight, float mortorSize);
    ~Brick() override;

    //glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const override;
    //bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const override;
    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _colorB, _colorM;
    float _brickWidth, _brickHeight, _mortorSize;
};
