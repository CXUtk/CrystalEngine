#pragma once
#include <vector>
#include <memory>
#include <Core/HitRecord.h>
#include <Utils/Ray.h>
#include <Primitives/Object.h>

class Scene {
public:
    Scene();
    ~Scene();

    bool Intersect(const Ray& ray, HitRecord* hitRecord) const;

private:
    std::vector<std::shared_ptr<Object>> _sceneObjects;
};
