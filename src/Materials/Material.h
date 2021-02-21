#pragma once
#include <glm/glm.hpp>
#include "Core/HitRecord.h"

class HitRecord;
class Material {
public:
    Material() {}
    virtual ~Material() = 0 {};

    virtual glm::vec3 Li(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) = 0;
    virtual glm::vec3 Le(const HitRecord& hitRecord, glm::vec3 wOut) { return glm::vec3(0); }
};
