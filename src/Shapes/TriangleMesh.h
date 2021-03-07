#pragma once
#include <Core/Object.h>
#include "Triangle.h"
#include <Accelerator/Accelerator.h>
#include <vector>

class TriangleMesh : public Object {
public:
    TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 transform, const std::shared_ptr<Material>& material,
        const std::shared_ptr<Light>& light);
    ~TriangleMesh() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    virtual const Light* GetLight() const override { return _light.get(); }
    virtual const Material* GetMaterial() const override { return _material.get(); }
    virtual void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;
    
    void SetMaterial(const std::shared_ptr<Material>& material) { _material = material; }

    //static std::shared_ptr<TriangleMesh> CreateQuad(std::shared_ptr<Material> material, glm::mat4 transform);
private:
    std::vector<std::shared_ptr<Object>> _triangles;
    BoundingBox _boundingBox;
    std::unique_ptr<Accelerator> _acceleator;

    std::shared_ptr<Material> _material;
    std::shared_ptr<Light> _light;

    std::vector<std::shared_ptr<Triangle>> transformed(const std::vector<std::shared_ptr<Triangle>>& triganles, glm::mat4 transform);
};
