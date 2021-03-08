#include "SamplerIntegrator.h"

SamplerIntegrator::SamplerIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : Integrator(), _camera(camera), _sampler(sampler) {
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
    for (int k = 0; k < numSamples; k++) {
        glm::vec2 offset = glm::vec2(0.5f, 0.5f);
        if (k != 0) {
            offset = glm::vec2(_sampler->NextFloat(), _sampler->NextFloat());
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                float u = (j + offset.x) / static_cast<float>(width);
                float v = (i + offset.y) / static_cast<float>(height);
                auto dir = camera->GetDir(u, v);
                _tmpColors[i * width + j] += Evaluate(Ray(eyePos, dir), scene);

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
