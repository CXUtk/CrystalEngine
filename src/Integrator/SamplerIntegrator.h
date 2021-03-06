#pragma once
#include "Integrator.h"
#include <Core/Camera.h>
#include <Sampler/Sampler.h>
#include <Textures/CubemapTexture.h>
class SamplerIntegrator : public Integrator {
public:
    SamplerIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, std::shared_ptr<CubemapTexture> skybox);
    virtual ~SamplerIntegrator() override {}

    void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer);
    virtual glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) = 0;

    std::shared_ptr<Camera> GetCamera() const { return _camera; }
    std::shared_ptr<Sampler> GetSampler() const { return _sampler; }
    std::shared_ptr<CubemapTexture> GetSkyBox() const { return _skyBox; }

private:
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Sampler> _sampler;
    std::shared_ptr<CubemapTexture> _skyBox;
    glm::vec3* _tmpColors;
};