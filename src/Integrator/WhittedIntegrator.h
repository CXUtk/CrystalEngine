#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class WhittedIntegrator : public SamplerIntegrator {
public:
    WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler);
    ~WhittedIntegrator() override;

    virtual void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) override;

private:
    Random _random;

    glm::vec3 evaluate(const Ray ray, std::shared_ptr<const Scene> scene);
};