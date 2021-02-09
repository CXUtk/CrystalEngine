#pragma once
#include <random>

class Random {
public:
    Random(unsigned int seed);
    Random() = default;

    void setSeed(unsigned int seed) { mt.seed(seed); }

    // Returns an int between range [a, b)
    int nextInt(int a, int b);

    // Returns a random floating point number between [0, 1)
    float nextFloat();


private:
    std::mt19937 mt;
};
