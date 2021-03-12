﻿#include "Phong.h"
#include <Reflection/PhongReflection.h>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
#include <Reflection/Lambertian.h>

static Random random;

Phong::Phong(glm::vec3 color) :_color(color), _uvExtend(1, 1) {
}

Phong::Phong(glm::vec3 color, glm::vec2 uvExt) : _color(color), _uvExtend(uvExt) {
}

Phong::~Phong() {
}

std::shared_ptr<BSDF> Phong::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    auto uv = isec.GetUV() * _uvExtend;
    bool a = fmod(uv.x, 0.5f) < 0.25f;
    bool b = fmod(uv.y, 0.5f) < 0.25f;
    auto color = _color;
    if (a ^ b) {
        color = glm::vec3(1);
    }
    auto N = isec.GetNormal();
    return std::make_shared<PhongReflection>(color, N);
}
