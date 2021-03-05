#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class PathTracingIntegrator : public SamplerIntegrator {
public:
    PathTracingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler);
    ~PathTracingIntegrator() override;

    glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) override;

private:
    Random _random;

    glm::vec3 evaluate(const Ray ray, std::shared_ptr<const Scene> scene);
    glm::vec3 sampleLight(const HitRecord& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, std::shared_ptr<Material> material);
    glm::vec3 sampleIndirect(const HitRecord& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, std::shared_ptr<Material> material);
};
