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
