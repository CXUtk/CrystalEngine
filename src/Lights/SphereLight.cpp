#include "SphereLight.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>

static Random random;

SphereLight::SphereLight(glm::vec3 pos, float r, glm::vec3 color, float power) : _pos(pos), _radius(r), _color(color), _power(power) {
    _surfaceArea = _radius * _radius * 4 * glm::pi<float>();
}

SphereLight::~SphereLight() {
}

glm::vec3 SphereLight::SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const {
    auto N = random.NextUnitVector();
    endpoint = _pos + _radius * N;
    auto Np = hit.GetHitPos() - endpoint;
    auto vp = glm::normalize(Np);

    // pdf = area projected on unit hemisphere / area of unit hemisphere
    *pdf = 1.0f / _surfaceArea;

    auto f = std::max(0.f, glm::dot(N, vp));
    if (f < 1e-6) return glm::vec3(0);
    // Power per surface area
    return _color * _power / _surfaceArea * f / glm::dot(Np, Np);
}

float SphereLight::GetArea() const {
    return _surfaceArea;
}
