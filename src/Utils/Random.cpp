#include "Random.h"

Random::Random(unsigned int seed) {
    mt.seed(seed);
}

int Random::NextInt(int a, int b) {
    return a + mt() % (b - a);
}

float Random::NextFloat() {
    return mt() / static_cast<float>(mt.max());
}
