#include "WhittedIntegrator.h"


static constexpr float EPS = 1e-5f;

WhittedIntegrator::WhittedIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, int maxDepth) : 
    SamplerIntegrator(camera, sampler),
    _maxDepth(maxDepth) {
}

WhittedIntegrator::~WhittedIntegrator() {
}

glm::vec3 WhittedIntegrator::Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) {
    return evaluate(ray, scene, 0);
}

//void WhittedIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {
//    auto camera = GetCamera();
//    auto eyePos = camera->GetEyePos();
//    int width = frameBuffer->GetWidth();
//    int height = frameBuffer->GetHeight();
//    int numSamples = GetSampler()->GetSamplesPerPixel();
//
//    int progress = 0;
//    for (int s = 0; s < 4; s++) {
//        for (int i = s; i < height; i += 4) {
//            for (int j = 0; j < width; j++) {
//                glm::vec3 radiance = glm::vec3(0);
//                for (int k = 0; k < numSamples; k++) {
//                    glm::vec2 offset = glm::vec2(0.5f, 0.5f);
//                    if (k != 0) {
//                        offset = glm::vec2(_random.NextFloat(), _random.NextFloat());
//                    }
//                    float u = (j + offset.x) / static_cast<float>(width);
//                    float v = (i + offset.y) / static_cast<float>(height);
//                    auto dir = camera->getDir(u, v);
//                    radiance += evaluate(Ray(eyePos, dir), scene, 0);
//                }
//                radiance /= numSamples;
//                frameBuffer->Lock();
//                frameBuffer->SetPixel(j, i, radiance);
//                frameBuffer->Unlock();
//            }
//            progress++;
//            printf("Tracing: %.2f %%\n", progress / static_cast<float>(height) * 100.f);
//        }
//    }
//}

glm::vec3 WhittedIntegrator::evaluate(const Ray ray, std::shared_ptr<const Scene> scene, int depth) {
    glm::vec3 L(0);
    if (depth == _maxDepth) return L;

    HitRecord hit;
    if (scene->Intersect(ray, &hit)) {
        glm::vec3 hitPos = hit.GetHitPos();
        glm::vec3 N = hit.GetNormal();
        glm::vec3 wOut = -ray.dir;
        auto material = hit.GetHitObject()->GetMaterial();
        L += material->Le(hit, wOut);

        glm::vec3 dir;
        float pdf;
        for (auto& light : scene->GetLights()) {
            glm::vec3 end;
            float pdf;
            auto raiance = light->SampleLi(hit, end, pdf);
            auto dir = glm::normalize(end - hit.GetHitPos());
            HitRecord hit2;
            float distance = glm::length(end - hit.GetHitPos());
            if (!scene->Intersect(Ray(hit.GetHitPos() + dir * EPS, dir), &hit2) || hit2.GetDistance() > distance) {
                L += raiance * material->BSDF(hit, wOut, dir) / pdf * std::max(0.f, glm::dot(N, dir));
            }
        }
        if (depth + 1 < _maxDepth && material->SampleDirection(hit, wOut, pdf, dir)) {
            auto radiance = evaluate(Ray(hitPos + N * EPS, dir), scene, depth + 1);
            L += radiance * material->BSDF(hit, wOut, dir) / pdf * std::max(0.f, glm::dot(N, dir));
        }
    }
    return L;
}
