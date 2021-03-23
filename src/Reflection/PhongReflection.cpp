#include "PhongReflection.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
static Random random;
PhongReflection::PhongReflection(glm::vec3 color, glm::vec3 normal)
    : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_DIFFUSE)), _color(color), _normal(normal) {

}

PhongReflection::~PhongReflection() {
}

glm::vec3 PhongReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    glm::vec3 V = wOut;
    glm::vec3 L = wIn;
    glm::vec3 N = _normal;
    glm::vec3 H = glm::normalize(V + L);

    glm::vec3 specular = glm::vec3(1) * std::pow(std::max(0.f, glm::dot(N, H)), 128.f);
    return _color / glm::pi<float>() + specular * 2.f / glm::pi<float>();
}

glm::vec3 PhongReflection::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    *pdf = 0;
    return glm::vec3(0);
}
