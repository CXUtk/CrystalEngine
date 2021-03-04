#pragma once
#include <glm/glm.hpp>
#include "Core/HitRecord.h"

class HitRecord;
class Material {
public:
    Material() {}
    virtual ~Material() = 0 {};

    virtual glm::vec3 BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) = 0;
    virtual glm::vec3 Le(const HitRecord& hitRecord, glm::vec3 wOut) { return glm::vec3(0); }
    virtual glm::vec3 SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf) {
        pdf = 1;
        return glm::vec3(0);
    }

    virtual glm::vec3 Merge(const HitRecord& hitRecord, const glm::vec3& wOut,  glm::vec3 Ldir, glm::vec3 LBSDF) {
        return Ldir + LBSDF;
    }
};
