#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class PathTracingIntegrator : public SamplerIntegrator {
public:
    PathTracingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, std::shared_ptr<CubemapTexture> skybox);
    ~PathTracingIntegrator() override;

    glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) override;

private:
    Random _random;

    glm::vec3 evaluate(const Ray ray, std::shared_ptr<const Scene> scene, int level);
    glm::vec3 sampleLight(const SurfaceInteraction& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, const std::shared_ptr<BSDF>& bsdf, int level);
    glm::vec3 sampleIndirect(const SurfaceInteraction& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, const std::shared_ptr<BSDF>& bsdf, int level);

    glm::vec3 emitted(const SurfaceInteraction& isec, const Object* object, glm::vec3 endpoint);

};
