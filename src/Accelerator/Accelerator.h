#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Primitives/Object.h"
#include "Utils/Ray.h"

class Accelerator {
public:
    Accelerator() {}
    virtual ~Accelerator() = 0 {}

    virtual void Build(const std::vector<std::shared_ptr<Object>>& objects) = 0;
    virtual bool Intersect(const Ray& ray, HitRecord* info) const = 0;

    static std::unique_ptr<Accelerator> GetAccelerator(const std::string& name);
};
