#pragma once
#include <glm/glm.hpp>
#include "Core/SurfaceInteraction.h"

class SurfaceInteraction;
class Material {
public:
    Material() {}
    virtual ~Material() = 0 {};

    virtual glm::vec3 BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const = 0;
    virtual glm::vec3 Le(const SurfaceInteraction& isec, glm::vec3 wOut) { return glm::vec3(0); }
    virtual bool SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const = 0;

    virtual glm::vec3 Merge(const SurfaceInteraction& isec, const glm::vec3& wOut,  glm::vec3 Ldir, glm::vec3 LBSDF) { return Ldir + LBSDF; }

    virtual void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const = 0;
};
