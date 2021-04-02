#include "PhongReflection.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
static Random random;
PhongReflection::PhongReflection(glm::vec3 color, glm::vec3 normal, glm::vec3 tangent)
    : BxDF(BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_DIFFUSE)), _color(color), _normal(normal), _tangent(tangent) {

}

PhongReflection::~PhongReflection() {
}

glm::vec3 PhongReflection::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    //glm::vec3 V = wOut;
    //glm::vec3 L = wIn;
    //glm::vec3 N = _normal;
    //glm::vec3 H = glm::normalize(V + L);

    //glm::vec3 specular = glm::vec3(1) * std::pow(std::max(0.f, glm::dot(N, H)), 128.f);
    //return _color / glm::pi<float>() + specular * 2.f / glm::pi<float>();
    return glm::vec3(0);
}

glm::vec3 PhongReflection::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    glm::vec3 R = glm::reflect(-wOut, _normal);
    glm::vec3 B = glm::normalize(glm::cross(_tangent, R));
    glm::vec3 T = glm::cross(R, B);
    glm::vec3 dir = random.NextUnitHemiSphereAlpha(1.f / (128 + 1));
    dir = dir.x * T + dir.y * R + dir.z * B;
    *wIn = glm::normalize(dir);
    *pdf = std::pow(std::max(0.f, glm::dot(dir, R)), 128.0f) * 129.f / glm::two_pi<float>();
    return _color;
}
