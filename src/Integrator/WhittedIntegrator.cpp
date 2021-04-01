﻿#include "WhittedIntegrator.h"


WhittedIntegrator::WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<CubemapTexture> skybox, int maxDepth) :
    SamplerIntegrator(camera, sampler),
    _maxDepth(maxDepth), _skyBox(skybox) {

}

WhittedIntegrator::~WhittedIntegrator() {
}

glm::vec3 WhittedIntegrator::Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) {
    return evaluate(ray, scene, 0);
}


glm::vec3 WhittedIntegrator::evaluate(const Ray& ray, std::shared_ptr<const Scene> scene, int depth) {
    glm::vec3 L(0);
    if (depth == _maxDepth) return L;
    SurfaceInteraction hit;
    if (scene->Intersect(ray, &hit)) {
        glm::vec3 hitPos = hit.GetHitPos();
        glm::vec3 N = hit.GetNormal();
        glm::vec3 wOut = -ray.dir;
        auto objHit = hit.GetHitObject();
        // Le light emitted
        L += emitted(hit, objHit, hitPos);

        auto bsdf = objHit->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) {
            return evaluate(hit.SpawnRay(ray.dir), scene, depth);
        }

        float pdf;
        for (auto& light : scene->GetLights()) {
            glm::vec3 end;
            auto raiance = light->SampleLi(hit, end, &pdf);
            auto dir = glm::normalize(end - hit.GetHitPos());
            SurfaceInteraction hit2;
            float distance = glm::length(end - hit.GetHitPos());
            if (!scene->IntersectTest(hit.SpawnRay(dir), 0, distance)) {
                L += raiance * bsdf->DistributionFunction(wOut, dir) / pdf * std::max(0.f, glm::dot(N, dir));
            }
        }

        glm::vec3 dir;
        auto reflectC = bsdf->SampleDirection(wOut, &dir, &pdf, BxDFType::BxDF_REFLECTION);
        if (depth + 1 < _maxDepth && pdf != 0 && reflectC != glm::vec3(0)) {
            auto radiance = evaluate(hit.SpawnRay(dir), scene, depth + 1);
            L += radiance * reflectC / pdf;
        }
        auto transmissionC = bsdf->SampleDirection(wOut, &dir, &pdf, BxDFType::BxDF_TRANSMISSION);
        if (depth + 1 < _maxDepth && pdf != 0 && transmissionC != glm::vec3(0)) {
            auto radiance = evaluate(hit.SpawnRay(dir), scene, depth + 1);
            L += radiance * transmissionC / pdf;
        }
        return L;
    }
    if (_skyBox == nullptr) return L;
    return _skyBox->GetTexel(ray.dir);
}

glm::vec3 WhittedIntegrator::emitted(const SurfaceInteraction& isec, const Object* object, glm::vec3 endpoint) {
    auto light = object->GetLight();
    if (!light)return glm::vec3(0);
    float pdf;
    auto Le = light->SampleLi(isec, endpoint, &pdf);
    return Le / pdf;
}
