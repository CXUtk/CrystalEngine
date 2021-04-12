#include "PhotonMPIntegrator.h"
#include <mutex>

static constexpr float pRR = 0.8f;
static constexpr float EPS = 1e-5;

PhotonMPIntegrator::PhotonMPIntegrator(std::shared_ptr<Camera> camera,
    std::shared_ptr<Sampler> sampler,
    std::shared_ptr<CubemapTexture> skybox) : Integrator(), _camera(camera), _sampler(sampler), _skyBox(skybox) {
    _tmpColors = nullptr;
    _photonMap = std::make_unique<PMKDTree>();
}

void PhotonMPIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {

    generateGlobalMap(scene);

    auto camera = GetCamera();
    auto eyePos = camera->GetEyePos();
    int width = frameBuffer->GetWidth();
    int height = frameBuffer->GetHeight();
    int numSamples = GetSampler()->GetSamplesPerPixel();

    _tmpColors = new glm::vec3[width * height];
    memset(_tmpColors, 0, sizeof(glm::vec3) * width * height);

    double progress = 0;
    double totalSamples = (double)numSamples * width * height;

    constexpr int NUM_THREADS = 6;
    std::shared_ptr<std::thread> threads[NUM_THREADS];

    std::mutex progressLock;

    for (int th = 0; th < NUM_THREADS; th++) {
        threads[th] = std::make_shared<std::thread>([&, th]() {
            for (int k = 0; k < numSamples; k++) {
                glm::vec2 offset = glm::vec2(0.5f, 0.5f);
                if (k != 0) {
                    offset = glm::vec2(_sampler->NextFloat(), _sampler->NextFloat());
                }

                for (int i = th; i < height; i += NUM_THREADS) {
                    for (int j = 0; j < width; j++) {
                        //i = 280, j = 228;
                        float u = (j + offset.x) / static_cast<float>(width);
                        float v = (i + offset.y) / static_cast<float>(height);
                        auto dir = camera->GetDir(u, v);
                        auto radiance = Evaluate(Ray(eyePos, dir), scene);
                        if (glm::isnan(radiance) != glm::bvec3(false)) {
                            radiance = glm::vec3(0);
                        }
                        _tmpColors[i * width + j] += radiance;

                        frameBuffer->Lock();
                        frameBuffer->SetPixel(j, i, _tmpColors[i * width + j] / static_cast<float>(k + 1));
                        frameBuffer->Unlock();
                    }
                }
                progressLock.lock();
                progress += width * ((height - th - 1) / NUM_THREADS + 1);
                progressLock.unlock();
                printf("Tracing: %.2lf %%\n", progress / totalSamples * 100.f);
            }
            });
    }

    for (int th = 0; th < NUM_THREADS; th++) {
        threads[th]->join();
    }
    delete[] _tmpColors;
}

void PhotonMPIntegrator::generateGlobalMap(std::shared_ptr<const Scene> scene) {
    for (const auto& light : scene->GetLights()) {
        glm::vec3 pos, dir;
        float pdf;
        for (int i = 0; i < 1000000; i++) {
            glm::vec3 Le = light->SampleEmission(&pos, &dir, &pdf);
            Photon p{ pos, dir, Le / pdf };
            photonTravel(Ray(pos, dir), scene, p, 0);
        }
    }
}

void PhotonMPIntegrator::photonTravel(const Ray& ray, std::shared_ptr<const Scene> scene, const glm::vec3& power, int depth) {
    if (depth > 6) return;
    SurfaceInteraction hit;
    if (scene->Intersect(ray, &hit)) {
        auto material = hit.GetHitObject()->GetMaterial();
        if (material == nullptr) return photonTravel(hit.SpawnRay(ray.dir, true), scene, power, depth);
        auto bsdf = material->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) return photonTravel(hit.SpawnRay(ray.dir, true), scene, power, depth);

        float pdf;
        glm::vec3 dir;
        auto brdf = bsdf->SampleDirection(-ray.dir, &dir, &pdf, BxDF_ALL);
        bool specular = (bsdf->Flags() & BxDF_SPECULAR) != 0;
        if (std::abs(pdf) < EPS) return;

        if (!specular) {
            Photon p;
            p.Pos = hit.GetHitPos();
            p.Dir = ray.dir;
            p.Power = power;
            _photonCollector.push_back(p);
        }
        else {
            return photonTravel(hit.SpawnRay(dir), scene, power * brdf, depth + 1);
        }
    }

}
