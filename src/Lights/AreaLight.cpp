#include "AreaLight.h"
#include <Utils/Random.h>
#include <glm/gtx/transform.hpp>

static Random random;

AreaLight::AreaLight(glm::vec3 pos, glm::vec3 u, glm::vec3 v, float power) : _pos(pos), _u(u), _v(v), _power(power) {

}

AreaLight::~AreaLight() {
}

glm::vec3 AreaLight::SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const {
    endpoint = _pos + random.NextFloat() * _u + random.NextFloat() * _v;
    auto normal = glm::normalize(glm::cross(_u, _v));
    auto d = hit.GetHitPos() - endpoint;
    auto dir = glm::normalize(d);

    auto area = glm::length(glm::cross(_u, _v));
    auto p = area * glm::pi<float>();
    auto cosine = std::max(0.f, glm::dot(dir, normal));
    *pdf = 1.f / area;
    return glm::vec3(_power) * cosine / p / glm::dot(d, d);
}


glm::vec3 AreaLight::SampleRadiance(glm::vec3 dir) const {
    auto normal = glm::normalize(glm::cross(_u, _v));
    return std::max(0.f, glm::dot(normal, dir)) * glm::vec3(_power / glm::pi<float>()) / glm::length(glm::cross(_u, _v));
}

glm::vec3 AreaLight::SampleEmission(glm::vec3* pos, glm::vec3* dir, float* pdf) const {
    auto normal = glm::normalize(glm::cross(_u, _v));
    *pos = _pos + random.NextFloat() * _u + random.NextFloat() * _v + normal * 1e-5f;

    auto T = glm::normalize(glm::cross(normal, _u));
    auto B = glm::normalize(glm::cross(normal, T));
    auto d = random.NextCosineUnitHemiSphere();
    *dir = T * d.x + normal * d.y + B * d.z;

    auto p = (glm::length(glm::cross(_u, _v)) * glm::pi<float>());
    auto cosine = std::max(0.f, d.y);
    *pdf = cosine / p;
    return glm::vec3(_power) / p * cosine;
}
