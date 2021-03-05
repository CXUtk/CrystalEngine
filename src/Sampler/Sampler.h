#pragma once
class Sampler {
public:
    Sampler(int samplesPerPixel) : _samplesPerPixel(samplesPerPixel) {}
    virtual ~Sampler() = 0 {}

    int GetSamplesPerPixel() const { return _samplesPerPixel; }

    virtual float NextFloat() = 0;
    void StartPixel(glm::ivec2 pixel);
    bool StartNextSample();
private:
    int _samplesPerPixel;
};