#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class EnvironmentIntegrator : public Integrator {
public:
    EnvironmentIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler);
    virtual ~EnvironmentIntegrator() override {}

    void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer);

    std::shared_ptr<Camera> GetCamera() const { return _camera; }
    std::shared_ptr<Sampler> GetSampler() const { return _sampler; }

private:
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Sampler> _sampler;
    glm::vec3* _tmpColors;
};
