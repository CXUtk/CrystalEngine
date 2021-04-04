#include "AreaLight.h"
#include <Utils/Random.h>
#include <glm/gtx/transform.hpp>

static Random random;

AreaLight::AreaLight(glm::vec3 pos, glm::vec3 u, glm::vec3 v, float emitPerArea) : _pos(pos), _u(u), _v(v), _emitPerArea(emitPerArea) {
    
}

AreaLight::~AreaLight() {
}

glm::vec3 AreaLight::SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const {
    endpoint = _pos + random.NextFloat() * _u + random.NextFloat() * _v;
    auto normal = glm::normalize(glm::cross(_u, _v));
    auto d = hit.GetHitPos() - endpoint;
    auto dir = glm::normalize(d);
    *pdf = 1 / glm::length(glm::cross(_u, _v));
    return glm::vec3(1 / glm::pi<float>()) * _emitPerArea * std::max(0.f, glm::dot(normal, dir)) / glm::dot(d, d);
}
