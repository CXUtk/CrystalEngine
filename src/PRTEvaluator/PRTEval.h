#pragma once
#include <glm/glm.hpp>
#include <string>
class SurfaceInteraction;
class PRTEval {
public:
    PRTEval() {}
    virtual ~PRTEval() = 0 {}

    virtual void Project(const glm::vec3& dir, const glm::vec3& Li, float omega) = 0;
    virtual void PrintCoefficient() const = 0;
    virtual glm::vec3 GetShading(const SurfaceInteraction& isec, glm::vec3 dir) const = 0;
    virtual void CalculateLight() {}
    virtual void ScaleBy(float scale) {}
};
