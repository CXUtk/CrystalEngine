#include "Phong.h"

Phong::Phong(glm::vec3 baseColor, float expK, glm::vec3 coeff) : _baseColor(baseColor), _expK(expK), _coeff(coeff) {
}

Phong::~Phong() {
}

glm::vec3 Phong::BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const {
    glm::vec3 ambient = _baseColor * 0.3f;
    glm::vec3 diffuse = _baseColor * 0.7f;
    glm::vec3 V = wOut;
    glm::vec3 L = wIn;
    glm::vec3 R = glm::reflect(-L, isec.GetNormal());

    glm::vec3 specular = glm::vec3(1) * std::pow(std::max(0.f, glm::dot(R, V)), _expK);
    return diffuse + specular;
}

bool Phong::SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const {
    return false;
}

glm::vec3 Phong::Le(const SurfaceInteraction& isec, glm::vec3 wOut) {
    return _baseColor * 0.1f;
}

void Phong::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const {
}
