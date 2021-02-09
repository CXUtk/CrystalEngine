#include "Random.h"

Random::Random(unsigned int seed) {
    mt.seed(seed);
}

int Random::nextInt(int a, int b) {
    return a + mt() % (b - a);
}

float Random::nextFloat() {
    return mt() / static_cast<float>(mt.max());
}
