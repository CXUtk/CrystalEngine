#include "SpecularReflection.h"
#include <Utils/Random.h>
#include <glm/gtx/transform.hpp>

static Random random;
SpecularReflection::SpecularReflection(glm::vec3 color, glm::vec3 normal)
    : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR)), _color(color), _normal(normal) {

}

SpecularReflection::~SpecularReflection() {
}

glm::vec3 SpecularReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return glm::vec3(0);
}

glm::vec3 SpecularReflection::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    *wIn = glm::normalize(glm::reflect(-wOut, _normal));
    *pdf = 1.0f;
    return _color;
}
