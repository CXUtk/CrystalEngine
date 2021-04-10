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
