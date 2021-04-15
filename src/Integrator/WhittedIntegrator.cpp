#include "WhittedIntegrator.h"
static constexpr float EPS = 1e-5;

WhittedIntegrator::WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<CubemapTexture> skybox, int maxDepth) :
    SamplerIntegrator(camera, sampler, skybox),
    _maxDepth(maxDepth) {

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

        if (depth == 0) {
            // Le light emitted
            L += emitted(hit, objHit, hitPos, -ray.dir);
        }

        auto material = objHit->GetMaterial();
        if (material == nullptr) return L;

        auto bsdf = objHit->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) {
            return evaluate(hit.SpawnRay(ray.dir), scene, depth);
        }

        for (auto& light : scene->GetLights()) {
            glm::vec3 lightP;
            float pdf;
            auto radiance = light->SampleLi(hit, lightP, &pdf);
            if (std::abs(pdf) < 1e-6) continue;

            auto v = lightP - hitPos;
            auto dir = glm::normalize(v);
            SurfaceInteraction hit2;

            float distance = glm::length(v);
            if (!scene->IntersectTest(Ray(hitPos + N * EPS, dir), 0, distance - EPS)) {
                L += radiance * bsdf->DistributionFunction(wOut, dir) * std::max(0.f, glm::dot(N, dir)) / pdf;
            }
        }
        if ((bsdf->Flags() & BxDFType::BxDF_DIFFUSE) != 0) return L;

        float pdf;
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
    if (GetSkyBox() == nullptr) return L;
    return GetSkyBox()->GetTexel(ray.dir);
}

glm::vec3 WhittedIntegrator::emitted(const SurfaceInteraction& isec, const Object* object, glm::vec3 endpoint, glm::vec3 dir) {
    auto light = object->GetLight();
    if (!light) return glm::vec3(0);
    auto Le = light->SampleRadiance(dir);
    return Le;
}
