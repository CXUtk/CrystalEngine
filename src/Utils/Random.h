#pragma once
#include <random>

class Random {
public:
    Random(unsigned int seed);
    Random() = default;

    void SetSeed(unsigned int seed) { mt.seed(seed); }

    // Returns an int between range [a, b)
    int NextInt(int a, int b);

    // Returns a random floating point number between [0, 1)
    float NextFloat();


private:
    std::mt19937 mt;
};
