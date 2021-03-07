#include "Cook_Torrance.h"
#include <Utils/Math.h>
#include <glm/gtx/transform.hpp>


#define SQR(x) ((x) * (x))


glm::vec3 fresnel(glm::vec3 f0, float cosTheta) {
    return f0 + (glm::vec3(1) - f0) * (1.f - std::pow(cosTheta, 5.f));
}


Cook_Torrance::Cook_Torrance(glm::vec3 baseColor, float roughness) : _baseColor(baseColor),
_roughness(roughness) {

}

Cook_Torrance::~Cook_Torrance() {
}

glm::vec3 Cook_Torrance::BSDF(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) {
    shouldBounce = true;
    glm::vec3 V = wOut;
    glm::vec3 L = wIn;
    glm::vec3 N = SurfaceInteraction.GetNormal();
    glm::vec3 H = glm::normalize(L + V);

    float NdotH = std::max(0.f, glm::dot(N, H));
    float NdotL = std::max(0.f, glm::dot(N, L));
    float NdotV = std::max(0.f, glm::dot(N, V));
    float VdotH = std::max(0.f, glm::dot(V, H));

    glm::vec3 diffuse = _baseColor / glm::pi<float>();
    float dom = std::max(1e-3f, (4 * NdotL * NdotV));
    float G = std::min({ 1.0f, 2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH });

    float rsq = _roughness * _roughness;
    float D = rsq / (glm::pi<float>() * SQR(NdotH * NdotH * (rsq - 1) + 1));
    auto F = fresnel(_baseColor, VdotH);

    auto sp = glm::max(D * G * F / dom, glm::vec3(0));
    return diffuse + sp;
}

glm::vec3 Cook_Torrance::Le(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut) {
    return _baseColor * 0.1f;
}
