#include "PointLight.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float power) : _pos(pos), _color(color), _power(power) {

}

PointLight::~PointLight() {
}

glm::vec3 PointLight::SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float& pdf) {
    endpoint = _pos;
    pdf = 1;
    auto v = endpoint - hit.GetHitPos();
    float distSQ = glm::dot(v, v);
    return _color * _power / distSQ;
}
