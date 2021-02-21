#pragma once
#include "Integrator.h"
#include <Core/Camera.h>
#include <Sampler/Sampler.h>
class SamplerIntegrator : public Integrator {
public:
    SamplerIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : Integrator(), _camera(camera), _sampler(sampler) {}
    virtual ~SamplerIntegrator() override {}

    virtual void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) = 0;
    std::shared_ptr<Camera> GetCamera() const { return _camera; }
    std::shared_ptr<Sampler> GetSampler() const { return _sampler; }
private:
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Sampler> _sampler;
};