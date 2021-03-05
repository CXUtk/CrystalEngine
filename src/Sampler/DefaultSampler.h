#pragma once
#include "Sampler.h"
class DefaultSampler : public Sampler {
public:
    DefaultSampler(int samplesPerPixel) : Sampler(samplesPerPixel), _random(114514) { }
    virtual ~DefaultSampler() {}

    float NextFloat() override { return _random.NextFloat(); }
private:
    Random _random;
};