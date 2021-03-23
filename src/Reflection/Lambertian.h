#pragma once
#include "BSDF.h"

class Lambertian : public BxDF {
public:
    Lambertian(glm::vec3 color, const glm::mat3& tnb);
    virtual ~Lambertian();

    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const override;
    glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const override;

private:
    glm::vec3 _color;
    glm::mat3 _tnb;
};

