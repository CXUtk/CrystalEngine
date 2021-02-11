#include "Raytracer.h"


Raytracer::Raytracer(int width, int height) : _width(width), _height(height) {
    _camera = std::shared_ptr<Camera>(new Camera(
        glm::vec3(0, 5, -5),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::pi<float>() / 3,
        width / static_cast<float>(height),
        1,
        1000
    ));

    _frameBuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height));
    _frameBuffer->Clear();

    _sampleCount = 1;
}

void Raytracer::Trace(std::shared_ptr<Scene> scene) {
    int progress = 0;
    for (int s = 0; s < 4; s++) {
        for (int i = s; i < _height; i += 4) {
            for (int j = 0; j < _width; j++) {
                float u = j / static_cast<float>(_width);
                float v = i / static_cast<float>(_height);
                auto dir = _camera->getDir(u, v);

                glm::vec3 color = glm::vec3(0);
                for (int k = 0; k < _sampleCount; k++) {
                    glm::vec2 offset = glm::vec2(0.5f, 0.5f);
                    if (k != 0) {
                        offset = glm::vec2(_random.nextFloat(), _random.nextFloat());
                    }
                    color += do_raycast(Ray(_camera->GetEyePos(), dir), scene);
                }
                color /= _sampleCount;
                _frameBuffer->Lock();
                _frameBuffer->SetPixel(j, i, color);
                _frameBuffer->unlock();
            }
            progress++;
            printf("Tracing: %.2f %%\n", progress / static_cast<float>(_height) * 100.f);
        }
    }


}

glm::vec3 Raytracer::do_raycast(const Ray& ray, std::shared_ptr<Scene> scene) {
    glm::vec3 color(0.3f, 0.6f, 1.0f);
    HitRecord hit;
    if (scene->Intersect(ray, &hit)) {
        color = hit.GetHitObject()->GetMaterial()->Shade();
    }
    return color;
}
