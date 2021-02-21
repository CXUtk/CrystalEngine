#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "Graphics/FrameBuffer.h"
#include "Utils/Random.h"
#include <Integrator/Integrator.h>


class Raytracer {
public:
    Raytracer(int width, int height);
    void Trace(std::shared_ptr<Scene> scene);
    std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return _frameBuffer; }

private:

    int _width, _height;
    int _sampleCount;

    std::shared_ptr<Camera> _camera;
    std::shared_ptr<FrameBuffer> _frameBuffer;
    std::shared_ptr<Integrator> _integrator;

    Random _random;
};
