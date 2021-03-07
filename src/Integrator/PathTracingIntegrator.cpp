#include "PathTracingIntegrator.h"

static constexpr float pRR = 0.8f;

PathTracingIntegrator::PathTracingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : SamplerIntegrator(camera, sampler) {
}

PathTracingIntegrator::~PathTracingIntegrator() {
}

glm::vec3 PathTracingIntegrator::Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) {
    glm::vec3 dirL(0), indirL(0);
    SurfaceInteraction hit;

    if (scene->Intersect(ray, &hit)) {
        auto material = hit.GetHitObject()->GetMaterial();
        dirL = sampleLight(hit, -ray.dir, scene, material);

        if (_random.NextFloat() > pRR) return dirL;//return material->Merge(hit, -ray.dir, dirL, indirL);

        indirL += sampleIndirect(hit, -ray.dir, scene, material);
        //return material->Merge(hit, -ray.dir, dirL, indirL);
    }
    return dirL + indirL;
}



glm::vec3 PathTracingIntegrator::evaluate(const Ray ray, std::shared_ptr<const Scene> scene) {
    return glm::vec3();
}

glm::vec3 PathTracingIntegrator::sampleLight(const SurfaceInteraction& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, const Material* material) {
    glm::vec3 N = hit.GetNormal();
    glm::vec3 hitPos = hit.GetHitPos();

    glm::vec3 L(0);
    for (auto& light : scene->GetLights()) {
        glm::vec3 lightP;
        float pdf;
        auto radiance = light->SampleLi(hit, lightP, pdf);

        auto v = lightP - hitPos;
        auto dir = glm::normalize(v);
        SurfaceInteraction hit2;
        bool shouldBounce = false;
        if (!scene->Intersect(Ray(hitPos + N * 0.0001f, dir), &hit2) || hit2.GetDistance() > glm::length(v)) {
            L += radiance * material->BSDF(hit, wOut, dir) / pdf * std::max(0.f, glm::dot(N, dir));
        }
    }
    return L;
}

glm::vec3 PathTracingIntegrator::sampleIndirect(const SurfaceInteraction& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, const Material* material) {
    return glm::vec3(0);
    //glm::vec3 N = hit.GetNormal();
    //glm::vec3 hitPos = hit.GetHitPos();

    //float pdf;
    //auto dir = material->SampleDirection(hit, wOut, pdf);

    //bool bounce = true;
    //auto bsdf = material->BSDF(hit, wOut, dir, bounce);
    //auto Le = material->Le(hit, wOut);
    //if (!bounce || bsdf == glm::vec3(0)) return Le;

    //auto Li = evaluate(Ray(hit.GetHitPos() + N * 0.0001f, dir), scene);
    //auto cosine = std::max(0.f, glm::dot(N, dir));
    //return Le + Li * bsdf * cosine / pRR / pdf;
}
