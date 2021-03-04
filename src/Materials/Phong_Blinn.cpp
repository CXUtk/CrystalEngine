#include "Phong_Blinn.h"

Phong_Blinn::Phong_Blinn(glm::vec3 baseColor, float expK, glm::vec3 coeff) : _baseColor(baseColor), _expK(expK), _coeff(coeff) {
}

Phong_Blinn::~Phong_Blinn() {
}

glm::vec3 Phong_Blinn::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) {
    shouldBounce = true;
    auto uv = hitRecord.GetUV();

    //bool a = std::fmod(uv.x * 5, 1.0f) < 0.5f;
    //bool b = std::fmod(uv.y * 5, 1.0f) < 0.5f;
    //auto color = glm::vec3(1, 0, 0);
    //if (a ^ b) {
    //    color = glm::vec3(1, 1, 0);
    //}
    glm::vec3 ambient = _baseColor * 0.3f;
    glm::vec3 diffuse = _baseColor * 0.7f;
    glm::vec3 V = wOut;
    glm::vec3 L = wIn;
    glm::vec3 N = hitRecord.GetNormal();
    glm::vec3 H = glm::normalize(V + L);

    glm::vec3 specular = glm::vec3(1) * std::pow(std::max(0.f, glm::dot(N, H)), _expK);
    return ambient + diffuse + specular;
}

glm::vec3 Phong_Blinn::Le(const HitRecord& hitRecord, glm::vec3 wOut) {
    return _baseColor * 0.1f;
}
