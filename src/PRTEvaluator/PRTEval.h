#pragma once
#include <glm/glm.hpp>
class SurfaceInteraction;
class PRTEval {
public:
    PRTEval() {}
    virtual ~PRTEval() = 0 {}

    virtual void Project(const glm::vec3& dir, const glm::vec3& Li, float omega) = 0;
    virtual void PrintCoefficient() const = 0;
    virtual glm::vec3 GetShading(const SurfaceInteraction& isec) const = 0;
    virtual void CalculateLight() {}
    virtual void ScaleBy(float scale) {}
};
