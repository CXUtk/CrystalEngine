#include "BSDF.h"
#include <Core/SurfaceInteraction.h>

BSDF::BSDF(const SurfaceInteraction* si) : _hit(si) {
}

void BSDF::AddBxDF(const std::shared_ptr<BxDF>& bxdf, glm::vec3 w) {
    _weights.push_back(w);
    _bxdfs.push_back(bxdf);
}

BxDFType BSDF::Flags() const {
    int sz = _bxdfs.size();
    int flag = 0;
    for (int i = 0; i < sz; i++) {
        flag |= _bxdfs[i]->GetType();
    }
    return (BxDFType)flag;
}

glm::vec3 BSDF::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    auto result = glm::vec3(0);
    bool reflect = glm::dot(_hit->GetNormal(), wIn) > 0;
    int sz = _bxdfs.size();
    for (int i = 0; i < sz; i++) {
        auto& bxdf = _bxdfs[i];
        if ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION)) || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))) {
            result += _weights[i] * bxdf->DistributionFunction(wOut, wIn);
        }
    }
    return result;
}

glm::vec3 BSDF::SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType sampleType) const {
    int sz = _bxdfs.size();
    *pdf = 0;
    *wIn = glm::vec3(0);
    glm::vec3 result = glm::vec3(0);
    for (int i = 0; i < sz; i++) {
        auto& bxdf = _bxdfs[i];
        if (bxdf->Contains(sampleType)) {
            glm::vec3 twIn;
            float tpdf;
            result += _weights[i] * bxdf->SampleDirection(wOut, &twIn, &tpdf);
            *wIn = twIn;
            *pdf += tpdf;
        }
    }
    return result;
}
