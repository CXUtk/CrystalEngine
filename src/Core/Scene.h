#pragma once
#include <vector>
#include <memory>
#include <Core/SurfaceInteraction.h>
#include <Utils/Ray.h>
#include <Core/Object.h>
#include <Lights/Light.h>
#include <Accelerator/Accelerator.h>

class Scene {
public:
    Scene();
    ~Scene();

    bool Intersect(const Ray& ray, SurfaceInteraction* SurfaceInteraction) const;
    bool IntersectWithLight(const Ray& ray, SurfaceInteraction* SurfaceInteraction) const;

    void AddObject(std::shared_ptr<Object> object);
    void AddLight(std::shared_ptr<Light> light);

    std::vector<std::shared_ptr<Light>> GetLights() const { return _lights; }

private:
    std::vector<std::shared_ptr<Object>> _sceneObjects;
    std::vector<std::shared_ptr<Light>> _lights;

    std::unique_ptr<Accelerator> _accelerator;
};
