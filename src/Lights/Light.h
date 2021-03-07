#pragma once
#include <Core/SurfaceInteraction.h>
class SurfaceInteraction;
class Light {
public:
    Light() {}
    virtual ~Light() = 0 {}
    virtual glm::vec3 SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float& pdf) = 0 {}
    virtual float GetArea() const { return 0; }
};