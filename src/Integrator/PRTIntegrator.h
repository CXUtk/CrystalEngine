#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class PRTIntegrator : public SamplerIntegrator {
public:
    PRTIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, std::shared_ptr<CubemapTexture> skybox);
    ~PRTIntegrator() override;

    void Preprocess(const std::shared_ptr<Scene>& scene) override;
    glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) override;

private:
    Random _random;
    std::shared_ptr<PRTEval> _prt;
};
