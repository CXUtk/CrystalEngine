#pragma once
#include <random>
#include <glm/glm.hpp>
class Random {
public:
    Random(unsigned int seed);
    Random() = default;

    void SetSeed(unsigned int seed) { mt.seed(seed); }

    // Returns an int between range [a, b)
    int NextInt(int a, int b);

    // Returns a random floating point number between [0, 1)
    float NextFloat();

    glm::vec3 NextUnitVector();
    glm::vec3 NextUnitHemiSphere();


private:
    std::mt19937 mt;
};
