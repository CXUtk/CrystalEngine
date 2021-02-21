#pragma once
#include "Utils/BoundingBox.h"
#include "Core/HitRecord.h"
#include "Materials/Material.h"

class Material;
class HitRecord;
class Object {
public:
    Object() {}
    virtual ~Object() = 0 {}

    virtual BoundingBox GetBoundingBox() const = 0;
    virtual bool Intersect(const Ray& ray, HitRecord* info) const = 0;
    virtual std::shared_ptr<Material> GetMaterial() const { return _material; }
    virtual void SetMaterial(std::shared_ptr<Material> material)  { _material = material; }

private:
    std::shared_ptr<Material> _material;
};
