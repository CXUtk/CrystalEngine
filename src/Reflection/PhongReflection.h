#pragma once
#include "BSDF.h"

class PhongReflection : public BSDF {
public:
    PhongReflection(glm::vec3 color, glm::vec3 normal);
    virtual ~PhongReflection();

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _color;
    glm::vec3 _normal;
};
