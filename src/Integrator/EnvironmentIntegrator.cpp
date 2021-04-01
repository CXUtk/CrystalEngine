#include "EnvironmentIntegrator.h"
#include "WhittedIntegrator.h"

static WhittedIntegrator* inte = nullptr;

EnvironmentIntegrator::EnvironmentIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler) : Integrator(),
_camera(camera), _sampler(sampler) {
    _tmpColors = nullptr;
    inte = new WhittedIntegrator(camera, sampler, nullptr, 5);
}

void EnvironmentIntegrator::Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) {

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
                float w = static_cast<float>(width);
                float u = (j) / (w);
                float v = (i) / w;

                float r = sqrt(u * u + v * v);
                float theta = v * glm::pi<float>();
                float phi = u * glm::two_pi<float>();


                // Consider only circle with r < 1
                auto dir = glm::vec3(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), cos(theta));

                //dir = glm::rotate(glm::half_pi<float>() / 2, glm::vec3(0, 0, 1)) * glm::vec4(dir, 1.0f);

                _tmpColors[i * width + j] += inte->Evaluate(Ray(eyePos, dir), scene);

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
