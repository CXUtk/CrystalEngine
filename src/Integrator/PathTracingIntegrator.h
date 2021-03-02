﻿#pragma once
#include "SamplerIntegrator.h"
#include <Utils/Random.h>

class PathTracingIntegrator : public SamplerIntegrator {
public:
    PathTracingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler);
    ~PathTracingIntegrator() override;

    virtual void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) override;

private:
    Random _random;
    glm::vec3* _tmpColors;

    glm::vec3 evaluate(const Ray ray, std::shared_ptr<const Scene> scene);
};
