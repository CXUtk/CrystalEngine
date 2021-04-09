#include "Glass.h"
#include <Reflection/SpecularTransmission.h>
#include <Reflection/SpecularReflection.h>

Glass::Glass(glm::vec3 baseColor, float eta) : _color(baseColor), _eta(eta) {
}

Glass::~Glass() {
}




std::shared_ptr<BSDF> Glass::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {


    float etaA = 1.0f, etaB = _eta;
    if (!isec.IsFrontFace()) std::swap(etaA, etaB);
    auto N = isec.GetNormal();
    std::shared_ptr<SpecularTransmission> trans = std::make_shared<SpecularTransmission>(_color, N, etaA, etaB);
    // auto reflect = std::make_shared<SpecularReflection>(_color, isec.GetNormal());
    auto bsdf = std::make_shared<BSDF>(&isec);
    // bsdf->AddBxDF(reflect, glm::vec3(0.2));
    bsdf->AddBxDF(trans, glm::vec3(1.0f));
    return bsdf;
}
//
//glm::vec3 Phong::BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const {
//    glm::vec3 ambient = _baseColor * 0.3f;
//    glm::vec3 diffuse = _baseColor * 0.7f;
//    glm::vec3 V = wOut;
//    glm::vec3 L = wIn;
//    glm::vec3 R = glm::reflect(-L, isec.GetNormal());
//
//    glm::vec3 specular = glm::vec3(1) * std::pow(std::max(0.f, glm::dot(R, V)), _expK);
//    return diffuse + specular;
//}
//
//bool Phong::SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir) const {
//    return false;
//}
//
//glm::vec3 Phong::Le(const SurfaceInteraction& isec, glm::vec3 wOut) {
//    return _baseColor * 0.1f;
//}
//
//void Phong::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const {
//}
