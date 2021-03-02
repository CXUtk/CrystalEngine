#include "WhittedIntegrator.h"

WhittedIntegrator::WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : SamplerIntegrator(camera, sampler) {
}

WhittedIntegrator::~WhittedIntegrator() {
}

void WhittedIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {
    auto camera = GetCamera();
    auto eyePos = camera->GetEyePos();
    int width = frameBuffer->GetWidth();
    int height = frameBuffer->GetHeight();
    int numSamples = GetSampler()->GetSamplesPerPixel();

    int progress = 0;
    for (int s = 0; s < 4; s++) {
        for (int i = s; i < height; i += 4) {
            for (int j = 0; j < width; j++) {
                glm::vec3 radiance = glm::vec3(0);
                for (int k = 0; k < numSamples; k++) {
                    glm::vec2 offset = glm::vec2(0.5f, 0.5f);
                    if (k != 0) {
                        offset = glm::vec2(_random.NextFloat(), _random.NextFloat());
                    }
                    float u = (j + offset.x) / static_cast<float>(width);
                    float v = (i + offset.y) / static_cast<float>(height);
                    auto dir = camera->getDir(u, v);
                    radiance += evaluate(Ray(eyePos, dir), scene);
                }
                radiance /= numSamples;
                frameBuffer->Lock();
                frameBuffer->SetPixel(j, i, radiance);
                frameBuffer->Unlock();
            }
            progress++;
            printf("Tracing: %.2f %%\n", progress / static_cast<float>(height) * 100.f);
        }
    }
}

glm::vec3 WhittedIntegrator::evaluate(const Ray ray, std::shared_ptr<const Scene> scene) {
    glm::vec3 L(0);
    HitRecord hit;
    if (scene->Intersect(ray, &hit)) {

        glm::vec3 N = hit.GetNormal();
        glm::vec3 wOut = -ray.dir;
        auto material = hit.GetHitObject()->GetMaterial();
        L += material->Le(hit, wOut);
        for (auto& light : scene->GetLights()) {
            glm::vec3 end;
            auto irraiance = light->SampleLi(hit, end);
            auto dir = glm::normalize(end - hit.GetHitPos());
            HitRecord hit2;
            float distance = glm::length(end - hit.GetHitPos());
            if (!scene->Intersect(Ray(hit.GetHitPos() + dir * 0.0001f, dir), &hit2) || hit2.GetDistance() > distance) {
                L += irraiance * material->BSDF(hit, wOut, dir) * std::max(0.f, glm::dot(N, dir));
            }
        }
    }
    return L;
}
