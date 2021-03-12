#pragma once
#include "BSDF.h"

class Transmission : public BSDF {
public:
    Transmission(glm::vec3 color, glm::vec3 normal);
    virtual ~Transmission();

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _color;
    glm::vec3 _normal;
};
