#pragma once
#include <Core/Scene.h>
#include <Graphics/FrameBuffer.h>
class Integrator {
public:
    Integrator(){}
    virtual ~Integrator() = 0 {}

    virtual void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer) = 0;
};