#include "PhotonMPIntegrator.h"
#include <mutex>
#include <Utils/Random.h>

static constexpr float pRR = 0.8f;
static constexpr float EPS = 1e-5;
static Random random;

PhotonMPIntegrator::PhotonMPIntegrator(std::shared_ptr<Camera> camera,
    std::shared_ptr<Sampler> sampler,
    std::shared_ptr<CubemapTexture> skybox) : Integrator(), _camera(camera), _sampler(sampler), _skyBox(skybox) {
    _tmpColors = nullptr;
    _photonMap = std::make_unique<PMKDTree>();
}

void PhotonMPIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {

    auto camera = GetCamera();
    auto eyePos = camera->GetEyePos();
    int width = frameBuffer->GetWidth();
    int height = frameBuffer->GetHeight();
    int numSamples = GetSampler()->GetSamplesPerPixel();

    _tmpColors = new glm::vec3[width * height];
    memset(_tmpColors, 0, sizeof(glm::vec3) * width * height);

    double progress = 0;
    double totalSamples = (double)numSamples * width * height;

    constexpr int NUM_THREADS = 1;
    std::shared_ptr<std::thread> threads[NUM_THREADS];

    std::mutex progressLock;
    generateGlobalMap(scene);

    for (int th = 0; th < NUM_THREADS; th++) {
        threads[th] = std::make_shared<std::thread>([&, th]() {
            for (int k = 0; k < numSamples; k++) {
                generateGlobalMap(scene);
                glm::vec2 offset = glm::vec2(0.5f, 0.5f);
                if (k != 0) {
                    offset = glm::vec2(_sampler->NextFloat(), _sampler->NextFloat());
                }

                for (int i = th; i < height; i += NUM_THREADS) {
                    for (int j = 0; j < width; j++) {
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

glm::vec3 PhotonMPIntegrator::Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) {
    return evaluate(ray, scene, 0);
}

void PhotonMPIntegrator::generateGlobalMap(std::shared_ptr<const Scene> scene) {
    constexpr float NUM_PHOTONS = 100000.f;
    for (const auto& light : scene->GetLights()) {
        glm::vec3 pos, dir;
        float pdf;
        for (int i = 0; i < NUM_PHOTONS; i++) {
            glm::vec3 Le = light->SampleEmission(&pos, &dir, &pdf) / NUM_PHOTONS;
            photonTravel(Ray(pos, dir), scene, Le / pdf, 0);
        }
    }
    _photonMap->Clear();
    _photonMap->Build(_photonCollector);
    _photonCollector.clear();
}

void PhotonMPIntegrator::photonTravel(const Ray& ray, std::shared_ptr<const Scene> scene, const glm::vec3& power, int depth) {
    SurfaceInteraction hit;
    if (scene->Intersect(ray, &hit)) {
        auto material = hit.GetHitObject()->GetMaterial();
        if (material == nullptr) return;
        auto bsdf = material->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) return;

        float pdf;
        glm::vec3 dir;
        auto brdf = bsdf->SampleDirection(-ray.dir, &dir, &pdf, BxDF_ALL);
        bool specular = (bsdf->Flags() & BxDF_SPECULAR) != 0;
        if (std::abs(pdf) < EPS) return;

        if (!specular) {
            Photon p{};
            p.Pos = hit.GetHitPos();
            p.Dir = ray.dir;
            p.Power = power;
            _photonCollector.push_back(p);
            if (random.NextFloat() > 0.7) {
                auto N = hit.GetNormal();
                return photonTravel(hit.SpawnRay(dir), scene, power * brdf / pdf * std::max(0.f, glm::dot(dir, N)), depth + 1);
            }
        }
        else {
            if (random.NextFloat() < pRR) {
                return photonTravel(hit.SpawnRay(dir), scene, power * brdf / pdf / pRR, depth + 1);
            }
        }
    }

}

glm::vec3 PhotonMPIntegrator::evaluate(const Ray& ray, std::shared_ptr<const Scene> scene, int depth) {
    glm::vec3 L(0);
    if (depth == 10) return L;
    SurfaceInteraction hit;
    if (scene->Intersect(ray, &hit)) {
        glm::vec3 hitPos = hit.GetHitPos();
        glm::vec3 N = hit.GetNormal();
        glm::vec3 wOut = -ray.dir;
        auto objHit = hit.GetHitObject();

        // Le light emitted
        auto light = objHit->GetLight();
        auto Le = glm::vec3(0);
        if (light != nullptr)
            Le = light->SampleRadiance(-ray.dir);
        L += Le;


        auto material = objHit->GetMaterial();
        if (material == nullptr) return L;

        auto bsdf = objHit->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) return L;

        if ((bsdf->Flags() & BxDFType::BxDF_DIFFUSE) != 0) {
            auto photons = _photonMap->NearestNeighbor(hitPos, 100);
            float r = 0;
            glm::vec3 radiance = glm::vec3(0);
            for (auto p : photons) {
                r = std::max(r, glm::length(p->Pos - hitPos));
                radiance += bsdf->DistributionFunction(-p->Dir, -ray.dir) * p->Power * std::max(0.f, glm::dot(-p->Dir, N));
            }
            return L + radiance / (glm::pi<float>() * r * r);
        }

        float pdf;
        glm::vec3 dir;
        auto reflectC = bsdf->SampleDirection(wOut, &dir, &pdf, BxDFType::BxDF_SPECULAR);
        if (depth + 1 < 10 && pdf != 0 && reflectC != glm::vec3(0)) {
            auto radiance = evaluate(hit.SpawnRay(dir), scene, depth + 1);
            L += radiance * reflectC / pdf;
        }
        return L;
    }
    if (GetSkyBox() == nullptr) return L;
    return GetSkyBox()->GetTexel(ray.dir);
}
