#pragma once
#include "BSDF.h"

class SpecularTransmission : public BxDF {
public:
    SpecularTransmission(glm::vec3 color, glm::vec3 normal, float etaA, float etaB);
    virtual ~SpecularTransmission();

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _color;
    glm::vec3 _normal;
    float _etaA, _etaB;
};
