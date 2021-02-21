#pragma once
class Sampler {
public:
    Sampler(int samplesPerPixel) : _samplesPerPixel(samplesPerPixel) {}
    virtual ~Sampler() = 0 {}

    int GetSamplesPerPixel() const { return _samplesPerPixel; }
private:
    int _samplesPerPixel;
};