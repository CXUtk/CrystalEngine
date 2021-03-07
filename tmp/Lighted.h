#pragma once
#include "Material.h"

class Lighted : public Material {
public:
    Lighted(glm::vec3 color);
    ~Lighted() override;

    glm::vec3 BSDF(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) override;
    glm::vec3 Le(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut) override;
private:
    glm::vec3 _color;
};
