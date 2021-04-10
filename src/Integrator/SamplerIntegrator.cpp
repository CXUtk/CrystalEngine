#include "SamplerIntegrator.h"
#include <mutex>

SamplerIntegrator::SamplerIntegrator(std::shared_ptr<Camera> camera,
    std::shared_ptr<Sampler> sampler,
    std::shared_ptr<CubemapTexture> skybox) : Integrator(), _camera(camera), _sampler(sampler), _skyBox(skybox) {
    _tmpColors = nullptr;
}

void SamplerIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {

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

                        //printf("Thread %d: %d, %d\n", th, i, j);
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
