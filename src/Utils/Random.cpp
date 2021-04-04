#include "Random.h"
#include <glm/gtx/transform.hpp>
Random::Random(unsigned int seed) {
    mt.seed(seed);
}

int Random::NextInt(int a, int b) {
    return a + mt() % (b - a);
}

float Random::NextFloat() {
    return mt() / static_cast<float>(mt.max());
}

glm::vec3 Random::NextUnitVector() {
    auto v = 2 * NextFloat() - 1;
    auto theta = NextFloat() * glm::two_pi<float>();
    auto r = std::sqrt(1 - v * v);
    return glm::vec3(r * std::cos(theta), v, r * std::sin(theta));
}

glm::vec3 Random::NextUnitHemiSphere() {
    auto v = NextFloat();
    auto theta = NextFloat() * glm::two_pi<float>();
    auto r = std::sqrt(1 - v * v);
    return glm::vec3(r * std::cos(theta), v, r * std::sin(theta));
}

glm::vec3 Random::NextUnitHemiSphereAlpha(float alpha) {
    auto v = std::pow(NextFloat(), alpha);
    auto theta = NextFloat() * glm::two_pi<float>();
    auto r = std::sqrt(1 - v * v);
    return glm::vec3(r * std::cos(theta), v, r * std::sin(theta));
}

glm::vec3 Random::NextCosineUnitHemiSphere() {
    auto r = std::sqrt(NextFloat());
    auto theta = NextFloat() * glm::two_pi<float>();

    auto x = r * std::cos(theta);
    auto z = r * std::sin(theta);
    return glm::vec3(x, std::sqrt(1 - r), z);
}

glm::vec3 Random::PerlinNoise(glm::vec2 uv) {
    glm::vec2 grid = glm::floor(uv);
    glm::vec2 xy = glm::fract(uv);
    xy = glm::smoothstep(glm::vec2(0), glm::vec2(1), xy);

    auto a = RandomFloat(grid);
    auto b = RandomFloat(grid + glm::vec2(1, 0));
    auto c = RandomFloat(grid + glm::vec2(0, 1));
    auto d = RandomFloat(grid + glm::vec2(1, 1));
    return glm::vec3(glm::mix(glm::mix(a, b, xy.x), glm::mix(c, d, xy.x), xy.y));
}

float Random::RandomFloat(glm::vec2 uv) {
    return glm::fract(std::sin(glm::dot(uv, glm::vec2(12.9898, 78.233))) * 43758.5453);
}
