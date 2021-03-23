#include "Transmission.h"

Transmission::Transmission(glm::vec3 color, glm::vec3 normal, float etaA, float etaB)
    : BxDF(BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR)), _color(color), _normal(normal),
    _etaA(etaA), _etaB(etaB) {
}

Transmission::~Transmission() {
}

glm::vec3 Transmission::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    return glm::vec3(0);
}

glm::vec3 Transmission::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const {
    *wIn = glm::refract(-wOut, _normal, _etaA / _etaB);
    *pdf = 1.0f;
    return _color;
}
