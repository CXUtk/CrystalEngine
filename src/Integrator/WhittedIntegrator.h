#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>
#include "Textures/CubemapTexture.h"

class WhittedIntegrator : public SamplerIntegrator {
public:
    WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, std::shared_ptr<CubemapTexture> skybox, int maxDepth);
    ~WhittedIntegrator() override;

    glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) override;

private:
    Random _random;
    int _maxDepth;

    glm::vec3 evaluate(const Ray& ray, std::shared_ptr<const Scene> scene, int depth);
    glm::vec3 emitted(const SurfaceInteraction& isec, const Object* object, glm::vec3 wOut);
};
