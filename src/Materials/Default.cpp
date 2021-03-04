#include "Default.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>

static Random random;

Default::Default(glm::vec3 color) :_color(color), _uvExtend(1, 1) {
}

Default::Default(glm::vec3 color, glm::vec2 uvExt) : _color(color), _uvExtend(uvExt) {
}

Default::~Default() {
}

glm::vec3 Default::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) {
    auto uv = hitRecord.GetUV() * _uvExtend;
    bool a = fmod(uv.x, 0.5f) < 0.25f;
    bool b = fmod(uv.y, 0.5f) < 0.25f;
    shouldBounce = true;
    if (a ^ b) {
        return glm::vec3(1) / glm::pi<float>();
    }
    return _color / glm::pi<float>();
}

glm::vec3 Default::SampleDirection(const HitRecord& hitRecord, glm::vec3 wOut, float& pdf) {
    auto N = hitRecord.GetNormal();
    auto tangent = glm::normalize(hitRecord.GetDpDu());
    auto bitangent = glm::normalize(glm::cross(N, tangent));

    auto v = random.NextUnitHemiSphere();
    // 1 / 2pi
    pdf = 0.5f / glm::pi<float>();
    return v.x * tangent + v.y * N + v.z * bitangent;
}
