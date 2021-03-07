#pragma once
#include "Object.h"
#include <Shapes/Shape.h>
#include <memory>

class GeometryObject : public Object {
public:
    GeometryObject(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material, 
        const std::shared_ptr<Light>& light);

    virtual BoundingBox GetBoundingBox() const override { return _shape->GetBoundingBox(); }
    virtual bool Intersect(const Ray& ray, SurfaceInteraction* isec) const override;
    virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override {
        return _shape->IntersectTest(ray, tMin, tMax);
    }

    virtual const Light* GetLight() const override { return _light.get(); }
    virtual const Material* GetMaterial() const override { return _material.get(); }

    virtual void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

private:
    std::shared_ptr<Shape> _shape;
    std::shared_ptr<Material> _material;
    std::shared_ptr<Light> _light;
};