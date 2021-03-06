﻿#pragma once
#include "Material.h"
#include "Textures/Texture.h"
#include <memory>

class Phong : public Material {
public:
    Phong(glm::vec3 color);
    Phong(glm::vec3 color, glm::vec2 uvExt);
    Phong(const std::shared_ptr<Texture>& texture);
    ~Phong() override;

    //glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const override;
    //bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const override;
    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

private:
    glm::vec3 _color;
    glm::vec2 _uvExtend;
    std::shared_ptr<Texture> _texture;
};
