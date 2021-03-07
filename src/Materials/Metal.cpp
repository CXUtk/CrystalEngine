#include "Metal.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
static Random random;
Metal::Metal(glm::vec3 color, float smoothness) : _color(color), _smoothness(smoothness) {

}
Metal::~Metal() {
}
glm::vec3 Metal::BSDF(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut, glm::vec3 wIn)const {
    glm::vec3 V = wOut;
    glm::vec3 N = SurfaceInteraction.GetNormal();
    glm::vec3 L = wIn;
    glm::vec3 H = glm::reflect(-L, N);

    //float cosi = std::max(glm::dot(N, L), 0.0f);
    //float cosr = std::max(glm::dot(N, V), 0.0f);

    //float alpha = 0.1f;
    //float k = std::max(0.f, glm::dot(H, N));
    //float part1 = std::exp(-(k * k) / (alpha * alpha));
    //float part2 = std::max(1e-6f, sqrt(cosi * cosr) * 4 * glm::pi<float>() * alpha * alpha);
    float k = std::max(0.f, glm::dot(H, V));
    return (float)(k > 0.95f ? std::pow(k, 64) : 0) * _color;
}
bool Metal::SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir)const {
    pdf = 1;
    auto R = glm::normalize(glm::reflect(-wOut, isec.GetNormal()));
    auto N = isec.GetNormal();
    auto tangent = glm::normalize(isec.GetDpDu());
    auto bitangent = glm::normalize(glm::cross(N, tangent));

    auto v = random.NextUnitHemiSphere();
    // 1 / 2pi
    auto O = v.x * tangent + v.y * N + v.z * bitangent;

    auto f = std::pow(random.NextFloat(), 1 / (_smoothness * _smoothness));

    dir = R;
    return true;
}


glm::vec3 Metal::Merge(const SurfaceInteraction& isec, const glm::vec3& wOut, glm::vec3 Ldir, glm::vec3 LBSDF) {
    return LBSDF;
}

void Metal::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const {
}
