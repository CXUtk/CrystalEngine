﻿#include "Lambertian.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
static Random random;
Lambertian::Lambertian(glm::vec3 color, const glm::mat3& tnb)
    : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_DIFFUSE)), _color(color), _tnb(tnb) {

}

Lambertian::~Lambertian() {
}

glm::vec3 Lambertian::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return _color / glm::pi<float>();
}

glm::vec3 Lambertian::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    // Law of TNB matrix, Tangent, Normal and BiTangent, all of them should be normalized
    glm::vec3 dir = random.NextCosineUnitHemiSphere();
    *wIn = _tnb * dir;
    *pdf = std::abs(dir.y) / glm::pi<float>();
    return DistributionFunction(wOut, *wIn);
}
