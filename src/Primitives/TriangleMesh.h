#pragma once
#include "Object.h"
#include "Triangle.h"
#include <Accelerator/Accelerator.h>
#include <vector>

class TriangleMesh : public Object {
public:
    TriangleMesh(const std::vector<std::shared_ptr<Triangle>> triangles, glm::mat4 transform);
    ~TriangleMesh() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;

    static std::shared_ptr<TriangleMesh> CreateQuad(std::shared_ptr<Material> material, glm::mat4 transform);


private:
    std::vector<std::shared_ptr<Triangle>> _triangles;
    BoundingBox _boundingBox;
    std::unique_ptr<Accelerator> _acceleator;

    void transform(glm::mat4 transform);
};
