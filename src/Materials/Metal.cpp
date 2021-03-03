#include "Metal.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>
Metal::Metal(glm::vec3 color) : _color(color) {
}
Metal::~Metal() {
}
glm::vec3 Metal::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) {
    glm::vec3 V = wOut;
    glm::vec3 N = hitRecord.GetNormal();
    glm::vec3 L = wIn;
    glm::vec3 H = glm::normalize(L + V);

    float cosi = std::max(glm::dot(N, L), 0.0f);
    float cosr = std::max(glm::dot(N, V), 0.0f);

    float alpha = 0.1f;
    float k = std::max(0.f, glm::dot(H, N));
    float part1 = std::exp(-(k * k) / (alpha * alpha));
    float part2 = std::max(1e-6f, sqrt(cosi * cosr) * 4 * glm::pi<float>() * alpha * alpha);
    return (_color ) / glm::pi<float>() + _color * part1 / part2;
}
