#include "Glass.h"
#include <Reflection/Transmission.h>
#include <Reflection/SpecularReflection.h>

Glass::Glass(glm::vec3 baseColor) : _color(baseColor) {
}

Glass::~Glass() {
}

std::shared_ptr<BSDF> Glass::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {

    std::shared_ptr<Transmission> trans;
    if (isec.IsFrontFace()) {
        trans = std::make_shared<Transmission>(_color, isec.GetNormal(), 1.0f, 1.333f);
    }
    else {
        trans = std::make_shared<Transmission>(_color, isec.GetNormal(), 1.333f, 1.0f);
    }
    auto reflect = std::make_shared<SpecularReflection>(_color, isec.GetNormal());
    auto bsdf = std::make_shared<BSDF>(&isec);
    bsdf->AddBxDF(reflect, glm::vec3(0.2));
    bsdf->AddBxDF(trans, glm::vec3(0.8));
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
