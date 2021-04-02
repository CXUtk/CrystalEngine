#include "Phong.h"
#include <Reflection/PhongReflection.h>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
#include <Reflection/Lambertian.h>
#include <Reflection/SpecularReflection.h>

static Random random;

Phong::Phong(glm::vec3 color) :_color(color), _uvExtend(1, 1), _texture(nullptr) {
}

Phong::Phong(glm::vec3 color, glm::vec2 uvExt) : _color(color), _uvExtend(uvExt), _texture(nullptr) {
}

Phong::Phong(const std::shared_ptr<Texture>& texture) : _color(glm::vec3(0)), _uvExtend(glm::vec2(1, 1)) {
    _texture = texture;
}


Phong::~Phong() {
}

std::shared_ptr<BSDF> Phong::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    auto uv = isec.GetUV() * _uvExtend;
    auto bsdf = std::make_shared<BSDF>(&isec);
    auto N = isec.GetNormal();
    auto T = glm::normalize(isec.GetDpDu());
    auto B = glm::normalize(glm::cross(N, T));
    auto tnb = glm::mat3(T, N, B);
    if (_texture == nullptr) {
        bool a = fmod(uv.x, 0.5f) < 0.25f;
        bool b = fmod(uv.y, 0.5f) < 0.25f;
        auto color = _color;
        if (a ^ b) {
            color = glm::vec3(1);
        }
        bsdf->AddBxDF(std::make_shared<Lambertian>(_color, tnb), glm::vec3(.8f));
        bsdf->AddBxDF(std::make_shared<PhongReflection>(_color, N, T), glm::vec3(1.0f));
    }
    else {
        bsdf->AddBxDF(std::make_shared<Lambertian>(_texture->GetTexel(uv), tnb), glm::vec3(.8f));
        bsdf->AddBxDF(std::make_shared<PhongReflection>(_texture->GetTexel(uv), N, T), glm::vec3(1));
    }
    return bsdf;
}
