#pragma once
#include "Sampler.h"
class DefaultSampler : public Sampler {
public:
    DefaultSampler(int samplesPerPixel) : Sampler(samplesPerPixel) {}
    virtual ~DefaultSampler() {}
private:
};