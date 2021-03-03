#include "Raytracer.h"
#include <Integrator/WhittedIntegrator.h>
#include <Integrator/PathTracingIntegrator.h>
#include <Sampler/DefaultSampler.h>

Raytracer::Raytracer(int width, int height) : _width(width), _height(height) {
    _camera = std::shared_ptr<Camera>(new Camera(
        glm::vec3(0, 3, 5),
        glm::vec3(0, 3, 0),
        glm::vec3(0, 1, 0),
        glm::pi<float>() / 3,
        width / static_cast<float>(height),
        1,
        1000
    ));

    _frameBuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height));
    _frameBuffer->Clear();

    auto sampler = std::make_shared<DefaultSampler>(1000);
    _integrator = std::shared_ptr<Integrator>(new PathTracingIntegrator(_camera, sampler));
}

void Raytracer::Trace(std::shared_ptr<Scene> scene) {
    _integrator->Render(scene, _frameBuffer);
}
