#include "Metal.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
static Random random;
Metal::Metal(glm::vec3 color, float smoothness) : _color(color), _smoothness(smoothness) {

}
Metal::~Metal() {
}
glm::vec3 Metal::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) {
    shouldBounce = true;
    glm::vec3 V = wOut;
    glm::vec3 N = hitRecord.GetNormal();
    glm::vec3 L = wIn;
    glm::vec3 H = glm::reflect(-L, N);

    //float cosi = std::max(glm::dot(N, L), 0.0f);
    //float cosr = std::max(glm::dot(N, V), 0.0f);

    //float alpha = 0.1f;
    //float k = std::max(0.f, glm::dot(H, N));
    //float part1 = std::exp(-(k * k) / (alpha * alpha));
    //float part2 = std::max(1e-6f, sqrt(cosi * cosr) * 4 * glm::pi<float>() * alpha * alpha);

    return _color;
}

glm::vec3 Metal::SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf) {

    pdf = 1;
    auto R = glm::normalize(glm::reflect(-wOut, hitRecord.GetNormal()));
    auto N = hitRecord.GetNormal();
    auto tangent = glm::normalize(hitRecord.GetDpDu());
    auto bitangent = glm::normalize(glm::cross(N, tangent));

    auto v = random.NextUnitHemiSphere();
    // 1 / 2pi
    auto O = v.x * tangent + v.y * N + v.z * bitangent;

    auto f = std::pow(random.NextFloat(), 1 / (_smoothness * _smoothness));

    return glm::mix(O, R, f);
}

glm::vec3 Metal::Merge(const HitRecord& hitRecord, const glm::vec3& wOut, glm::vec3 Ldir, glm::vec3 LBSDF) {
    return LBSDF;
}
