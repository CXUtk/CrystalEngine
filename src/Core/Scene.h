#pragma once
#include <vector>
#include <memory>
#include <Core/HitRecord.h>
#include <Utils/Ray.h>
#include <Primitives/Object.h>
#include <Lights/Light.h>
#include <Accelerator/Accelerator.h>

class Scene {
public:
    Scene();
    ~Scene();

    bool Intersect(const Ray& ray, HitRecord* hitRecord) const;
    bool IntersectWithLight(const Ray& ray, HitRecord* hitRecord) const;

    void AddObject(std::shared_ptr<Object> object);
    void AddLight(std::shared_ptr<Light> light);

    std::vector<std::shared_ptr<Light>> GetLights() const { return _lights; }

private:
    std::vector<std::shared_ptr<Object>> _sceneObjects;
    std::vector<std::shared_ptr<Light>> _lights;

    std::unique_ptr<Accelerator> _accelerator;
};
