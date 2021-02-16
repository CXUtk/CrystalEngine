#pragma once
#include <memory>
#include <vector>
#include "Primitives/Object.h"
#include "Utils/Ray.h"

class Accelerator {
public:
    Accelerator() {}
    virtual ~Accelerator() = 0 {}

    virtual void Build(const std::vector<std::shared_ptr<Object>>& objects) = 0;
    virtual bool Intersect(const Ray& ray, HitRecord* info) = 0;
};