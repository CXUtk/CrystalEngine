#include "PathTracingIntegrator.h"

PathTracingIntegrator::PathTracingIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : SamplerIntegrator(camera, sampler) {
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

        glm::vec3 N = hit.GetNormal();
        glm::vec3 wOut = -ray.dir;
        glm::vec3 hitPos = hit.GetHitPos();

        auto material = hit.GetHitObject()->GetMaterial();

        for (auto& light : scene->GetLights()) {
            glm::vec3 lightP;
            float pdf;
            auto radiance = light->SampleLi(hit, lightP, pdf);

            auto v = lightP - hitPos;
            auto dir = glm::normalize(v);
            HitRecord hit2;
            if (!scene->Intersect(Ray(hitPos + N * 0.0001f, dir), &hit2) || hit2.GetDistance() > glm::length(v)) {
                dirL += radiance * material->BSDF(hit, wOut, dir)  / pdf * std::max(0.f, glm::dot(N, dir));
            }
        }

        if (_random.NextFloat() > 0.8f) return dirL + indirL;
        //glm::vec3 T = glm::normalize(hit.GetDpDu());
        //glm::vec3 B = glm::normalize(glm::cross(N, T));
        glm::vec3 dir = _random.NextUnitVector();
        while (glm::dot(dir, N) <= 0) {
            dir = _random.NextUnitVector();
        }

        auto bsdf = material->BSDF(hit, wOut, dir);
        if (bsdf == glm::vec3(0)) return dirL + indirL;

        auto Li = evaluate(Ray(hit.GetHitPos() + N * 0.0001f, dir), scene);
        auto cosine = std::max(0.f, glm::dot(N, dir));
        indirL += Li * bsdf * cosine / 0.8f * glm::two_pi<float>();
    }
    return dirL + indirL;
}
