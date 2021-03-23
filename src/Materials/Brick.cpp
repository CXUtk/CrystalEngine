#include <Reflection/PhongReflection.h>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
#include <Reflection/Lambertian.h>
#include "Brick.h"

static Random random;


Brick::Brick(glm::vec3 colorBrick, glm::vec3 colorMortor, float brickWidth, float brickHeight, float mortorSize) 
: _colorB(colorBrick), _colorM(colorMortor), _brickWidth(brickWidth), _brickHeight(brickHeight), _mortorSize(mortorSize) {

}

Brick::~Brick() {
}

std::shared_ptr<BSDF> Brick::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    glm::vec2 blockSize = glm::vec2(_brickWidth + _mortorSize, _brickHeight + _mortorSize);
    auto uv = isec.GetUV();
    int row = (int)(uv.y / blockSize.y);
    glm::vec2 offset = glm::vec2(0);
    if (row & 1) {
        offset.x = blockSize.x * 0.5f;
    }
    glm::vec2 rem = glm::mod(uv + offset, blockSize);
    glm::vec3 color = _colorB;
    if (rem.x > _brickWidth || rem.y > _brickHeight) {
        color = _colorM;
    }
    auto T = glm::normalize(isec.GetDpDu());
    auto N = isec.GetNormal();
    auto B = glm::normalize(glm::cross(N, T));
    auto tnb = glm::mat3(T, N, B);
    auto f = random.PerlinNoise(uv * 200.f) * 0.5f;

    auto bsdf = std::make_shared<BSDF>(&isec);
    bsdf->AddBxDF(std::make_shared<Lambertian>(color * 1.3f - glm::vec3(f), tnb), glm::vec3(1));
    return bsdf;
}
