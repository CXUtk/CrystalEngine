#include "PointLight.h"
#include <Utils/Random.h>
#include <glm/gtx/transform.hpp>
static Random random;

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float power) : _pos(pos), _color(color), _power(power) {

}

PointLight::~PointLight() {
}

glm::vec3 PointLight::SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const {
    endpoint = _pos;
    *pdf = 1;
    auto v = endpoint - hit.GetHitPos();
    float distSQ = glm::dot(v, v);
    return _color * _power / (4 * glm::pi<float>() * distSQ);
}

glm::vec3 PointLight::SampleRadiance(glm::vec3 dir) const {
    return _power * _color / (4 * glm::pi<float>());
}


glm::vec3 PointLight::SampleEmission(glm::vec3* pos, glm::vec3* dir, float* pdf) const {
    *pos = _pos;
    *dir = random.NextUnitVector();
    *pdf = .5f / glm::two_pi<float>();
    return _power * _color / (2 * glm::two_pi<float>());
}
