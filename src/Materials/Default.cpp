#include "Default.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
#include <Reflection/Lambertian.h>
#include <Reflection/PhongReflection.h>

static Random random;

Default::Default(glm::vec3 color) :_color(color), _color2(glm::vec3(1)), _uvExtend(1, 1) {
}

Default::Default(glm::vec3 color, glm::vec2 uvExt) : _color(color), _color2(glm::vec3(1)), _uvExtend(uvExt) {
}

Default::Default(glm::vec3 color, glm::vec3 color2, glm::vec2 uvExt) 
    : _color(color), _color2(color2), _uvExtend(uvExt) {
}


Default::~Default() {
}

std::shared_ptr<BSDF> Default::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    auto uv = isec.GetUV() * _uvExtend;
    bool a = fmod(uv.x, 0.5f) < 0.25f;
    bool b = fmod(uv.y, 0.5f) < 0.25f;
    auto color = _color;
    if (a ^ b) {
        color = _color2;
    }
    auto T = glm::normalize(isec.GetDpDu());
    auto N = isec.GetNormal();
    auto B = glm::normalize(glm::cross(N, T));
    auto tnb = glm::mat3(T, N, B);
    return std::make_shared<Lambertian>(color, tnb);
}


