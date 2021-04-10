#include "SpecularTransmission.h"
#include <Utils/Random.h>

static Random random;
static bool Refract(glm::vec3 N, glm::vec3 I, glm::vec3& T, float etaA, float etaB) {
    auto cosI = glm::dot(N, I);
    auto sinI = std::sqrt(1.0f - cosI * cosI);
    auto sinT = etaA * sinI / etaB;
    auto sin2T = sinT * sinT;
    if (sin2T >= 1.0) return false;
    auto cosT = std::sqrt(1.0f - sin2T);
    auto tangent = -glm::normalize(I - cosI * N);
    T = -N * cosT + tangent * sinT;
    return true;
}

static float Fresnel(glm::vec3 N, glm::vec3 I, glm::vec3 T, float etaA, float etaB) {
    auto cosI = glm::dot(N, I);
    auto cosT = glm::dot(-N, T);
    float r1 = (etaB * cosI - etaA * cosT) / (etaB * cosI + etaA * cosT);
    float r2 = (etaA * cosI - etaB * cosT) / (etaA * cosI + etaB * cosT);
    return 0.5 * (r1 * r1 + r2 * r2);
}

SpecularTransmission::SpecularTransmission(glm::vec3 color, glm::vec3 normal, float etaA, float etaB)
    : BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR)), _color(color), _normal(normal),
    _etaA(etaA), _etaB(etaB) {
}

SpecularTransmission::~SpecularTransmission() {
}

glm::vec3 SpecularTransmission::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return glm::vec3(0);
}

glm::vec3 SpecularTransmission::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    glm::vec3 tDir;
    if (Refract(_normal, wOut, tDir, _etaA, _etaB)) {
        float fr = Fresnel(_normal, wOut, tDir, _etaA, _etaB);
        if (random.NextFloat() < fr) {
            *wIn = glm::reflect(-wOut, _normal);
        }
        else {
            *wIn = tDir;// glm::refract(-wOut, _normal, _etaA / _etaB);
        }
    }
    else {
        *wIn = glm::reflect(-wOut, _normal);
    }
    *pdf = 1.0f;
    return _color;
}
