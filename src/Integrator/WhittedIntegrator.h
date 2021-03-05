#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class WhittedIntegrator : public SamplerIntegrator {
public:
    WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, int maxDepth);
    ~WhittedIntegrator() override;

    glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) override;

private:
    Random _random;
    int _maxDepth;

    glm::vec3 evaluate(const Ray ray, std::shared_ptr<const Scene> scene, int depth);
};