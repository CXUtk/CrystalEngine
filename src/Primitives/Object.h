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
    virtual void ApplyTransform(glm::mat4 transform) = 0;
    virtual glm::vec3 GetNormal(glm::vec3 hitpos, glm::vec3 rayDir) const = 0;

private:
    std::shared_ptr<Material> _material;
};
