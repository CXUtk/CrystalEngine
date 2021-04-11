#include "PathTracingIntegrator.h"

static constexpr float pRR = 0.8f;
static constexpr float EPS = 1e-5;

PathTracingIntegrator::PathTracingIntegrator(std::shared_ptr<Camera> camera,
    std::shared_ptr<Sampler> sampler,
    std::shared_ptr<CubemapTexture> skybox) : SamplerIntegrator(camera, sampler, skybox) {
}

PathTracingIntegrator::~PathTracingIntegrator() {
}

glm::vec3 PathTracingIntegrator::Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) {
    return evaluate(ray, scene, 0, true);

}


glm::vec3 PathTracingIntegrator::evaluate(const Ray& ray, std::shared_ptr<const Scene> scene, int level, bool specular) {
    glm::vec3 dirL(0), indirL(0);
    SurfaceInteraction hit;
    float beta = (level > 3) ? pRR : 1.0f;
    if (scene->Intersect(ray, &hit)) {
        if (level == 0 || specular)
            dirL = emitted(hit, hit.GetHitObject(), hit.GetHitPos());

        auto material = hit.GetHitObject()->GetMaterial();
        if (material == nullptr) return dirL;
        auto bsdf = material->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) return dirL;

        dirL += sampleLight(hit, -ray.dir, scene, bsdf, level);

        if (level > 3 && _random.NextFloat() > pRR) return dirL;//return material->Merge(hit, -ray.dir, dirL, indirL);

        indirL += sampleIndirect(hit, -ray.dir, scene, bsdf, level, specular);
        //return material->Merge(hit, -ray.dir, dirL, indirL);

    }
    else {
        if (GetSkyBox() == nullptr) return dirL;
        return GetSkyBox()->GetTexel(ray.dir);
    }
    return (dirL + indirL);
}

glm::vec3 PathTracingIntegrator::sampleLight(const SurfaceInteraction& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, const std::shared_ptr<BSDF>& bsdf, int level) {
    glm::vec3 L(0);
    if (bsdf == nullptr) {
        return L;
    }
    glm::vec3 N = hit.GetNormal();
    glm::vec3 hitPos = hit.GetHitPos();
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
    return L;
}

glm::vec3 PathTracingIntegrator::sampleIndirect(const SurfaceInteraction& hit, glm::vec3 wOut,
    std::shared_ptr<const Scene> scene,
    const std::shared_ptr<BSDF>& bsdf, int level, bool specular) {
    if (bsdf == nullptr) {
        return glm::vec3(0);
    }
    glm::vec3 N = hit.GetNormal();
    glm::vec3 hitPos = hit.GetHitPos();

    float pdf;
    glm::vec3 dir;
    auto brdf = bsdf->SampleDirection(wOut, &dir, &pdf, BxDF_ALL);
    specular &= (bsdf->Flags() & BxDF_SPECULAR) != 0;
    if (std::abs(pdf) < EPS) return glm::vec3(0);


    auto Li = evaluate(hit.SpawnRay(dir), scene, level + 1, specular);
    auto cosine = specular ? 1.0f : std::max(0.f, glm::dot(N, dir));

    return Li * brdf * cosine / pdf;
}

glm::vec3 PathTracingIntegrator::emitted(const SurfaceInteraction& isec, const Object* object, glm::vec3 endpoint) {
    auto light = object->GetLight();
    if (light == nullptr) return glm::vec3(0);
    auto Le = light->IntensityPerArea();
    return Le;
}
