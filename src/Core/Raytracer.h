#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "Graphics/FrameBuffer.h"
#include "Utils/Random.h"


class Raytracer {
public:
    Raytracer(int width, int height);
    void Trace(std::shared_ptr<Scene> scene);
    std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return _frameBuffer; }

private:
    glm::vec3 do_raycast(const Ray& ray, std::shared_ptr<Scene> scene);


    int _width, _height;
    int _sampleCount;

    std::shared_ptr<Camera> _camera;
    std::shared_ptr<FrameBuffer> _frameBuffer;

    Random _random;
};
