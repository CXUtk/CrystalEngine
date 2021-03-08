#pragma once
#include <Utils/BoundingBox.h>
#include <Materials/Material.h>
#include <Lights/Light.h>
#include "SurfaceInteraction.h"
#include <Reflection/BSDF.h>

class SurfaceInteraction;
class Light;
class Material;

class Object {
public:
    Object() {}
    virtual ~Object() = 0 {}

    virtual BoundingBox GetBoundingBox() const = 0;
    virtual bool Intersect(const Ray& ray, SurfaceInteraction* isec) const = 0;
    virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;

    virtual const Light* GetLight() const { return nullptr; }
    virtual const Material* GetMaterial() const { return nullptr; }

    virtual std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const = 0;
};
