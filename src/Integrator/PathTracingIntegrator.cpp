#include "PathTracingIntegrator.h"

static constexpr float pRR = 0.8f;

PathTracingIntegrator::PathTracingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : SamplerIntegrator(camera, sampler) {
    _tmpColors = nullptr;
}

PathTracingIntegrator::~PathTracingIntegrator() {
}

void PathTracingIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {
    auto camera = GetCamera();
    auto eyePos = camera->GetEyePos();
    int width = frameBuffer->GetWidth();
    int height = frameBuffer->GetHeight();
    int numSamples = GetSampler()->GetSamplesPerPixel();

    _tmpColors = new glm::vec3[width * height];
    memset(_tmpColors, 0, sizeof(glm::vec3) * width * height);

    double progress = 0;
    double totalSamples = (double)numSamples * width * height;
    for (int k = 0; k < numSamples; k++) {
        glm::vec2 offset = glm::vec2(0.5f, 0.5f);
        if (k != 0) {
            offset = glm::vec2(_random.NextFloat(), _random.NextFloat());
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                float u = (j + offset.x) / static_cast<float>(width);
                float v = (i + offset.y) / static_cast<float>(height);
                auto dir = camera->getDir(u, v);
                _tmpColors[i * width + j] += evaluate(Ray(eyePos, dir), scene);

                frameBuffer->Lock();
                frameBuffer->SetPixel(j, i, _tmpColors[i * width + j] / static_cast<float>(k + 1));
                frameBuffer->Unlock();
            }
            progress += width;
        }
        printf("Tracing: %.2lf %%\n", progress / totalSamples * 100.f);
    }
    delete[] _tmpColors;
}

glm::vec3 PathTracingIntegrator::evaluate(const Ray ray, std::shared_ptr<const Scene> scene) {
    glm::vec3 dirL(0), indirL(0);
    HitRecord hit;

    if (scene->Intersect(ray, &hit)) {
        auto material = hit.GetHitObject()->GetMaterial();
        dirL = sampleLight(hit, -ray.dir, scene, material);

        if (_random.NextFloat() > pRR) return material->Merge(hit, -ray.dir, dirL, indirL);

        indirL += sampleIndirect(hit, -ray.dir, scene, material);
        return material->Merge(hit, -ray.dir, dirL, indirL);
    }
    return dirL + indirL;
}

glm::vec3 PathTracingIntegrator::sampleLight(const HitRecord& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, std::shared_ptr<Material> material) {
    glm::vec3 N = hit.GetNormal();
    glm::vec3 hitPos = hit.GetHitPos();

    glm::vec3 L(0);
    for (auto& light : scene->GetLights()) {
        glm::vec3 lightP;
        float pdf;
        auto radiance = light->SampleLi(hit, lightP, pdf);

        auto v = lightP - hitPos;
        auto dir = glm::normalize(v);
        HitRecord hit2;
        bool shouldBounce = false;
        if (!scene->Intersect(Ray(hitPos + N * 0.0001f, dir), &hit2) || hit2.GetDistance() > glm::length(v)) {
            L += radiance * material->BSDF(hit, wOut, dir, shouldBounce) / pdf * std::max(0.f, glm::dot(N, dir));
        }
    }
    return L;
}

glm::vec3 PathTracingIntegrator::sampleIndirect(const HitRecord& hit, glm::vec3 wOut, std::shared_ptr<const Scene> scene, std::shared_ptr<Material> material) {
    glm::vec3 N = hit.GetNormal();
    glm::vec3 hitPos = hit.GetHitPos();

    float pdf;
    auto dir = material->SampleDirection(hit, wOut, pdf);

    bool bounce = true;
    auto bsdf = material->BSDF(hit, wOut, dir, bounce);
    auto Le = material->Le(hit, wOut);
    if (!bounce || bsdf == glm::vec3(0)) return Le;

    auto Li = evaluate(Ray(hit.GetHitPos() + N * 0.0001f, dir), scene);
    auto cosine = std::max(0.f, glm::dot(N, dir));
    return Le + Li * bsdf * cosine / pRR / pdf;
}
