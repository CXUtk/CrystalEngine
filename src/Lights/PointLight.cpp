#include "PointLight.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float power) : _pos(pos), _color(color), _power(power) {

}

PointLight::~PointLight() {
}

glm::vec3 PointLight::SampleLi(const HitRecord& hit, glm::vec3& endpoint) {
    endpoint = _pos;
    auto v = endpoint - hit.GetHitPos();
    float distSQ = glm::dot(v, v);
    return _color * _power / distSQ;
}
