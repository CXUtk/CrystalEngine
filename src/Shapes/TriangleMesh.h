#pragma once
#include <Core/Object.h>
#include "Triangle.h"
#include <Accelerator/Accelerator.h>
#include <vector>

class TriangleMesh : public Shape {
public:
    TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 transform);
    ~TriangleMesh() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

    //static std::shared_ptr<TriangleMesh> CreateQuad(std::shared_ptr<Material> material, glm::mat4 transform);
private:
    std::vector<std::shared_ptr<Triangle>> _triangles;
    BoundingBox _boundingBox;
    std::unique_ptr<Accelerator> _acceleator;

    void transform(glm::mat4 transform);
};
