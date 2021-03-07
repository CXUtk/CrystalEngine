﻿#include "Default.h"
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>

static Random random;

Default::Default(glm::vec3 color) :_color(color), _uvExtend(1, 1) {
}

Default::Default(glm::vec3 color, glm::vec2 uvExt) : _color(color), _uvExtend(uvExt) {
}

Default::~Default() {
}

glm::vec3 Default::BSDF(const SurfaceInteraction& isec, glm::vec3 wOut, glm::vec3 wIn) const {
    auto uv = isec.GetUV() * _uvExtend;
    bool a = fmod(uv.x, 0.5f) < 0.25f;
    bool b = fmod(uv.y, 0.5f) < 0.25f;
    if (a ^ b) {
        return glm::vec3(1);
    }
    return _color;
}

bool Default::SampleDirection(const SurfaceInteraction& isec, glm::vec3 wOut, float& pdf, glm::vec3& dir)const {
    auto N = isec.GetNormal();
    auto tangent = glm::normalize(isec.GetDpDu());
    auto bitangent = glm::normalize(glm::cross(N, tangent));

    auto v = random.NextUnitHemiSphere();
    // 1 / 2pi
    pdf = 0.5f / glm::pi<float>();
    dir = v.x* tangent + v.y * N + v.z * bitangent;
    return false;
}

void Default::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const {
}

