#pragma once
#include "Object.h"
#include "Triangle.h"
#include <vector>

class TriangleMesh : public Object {
public:
    TriangleMesh() = default;
    TriangleMesh(const std::vector<std::shared_ptr<Triangle>> triangles);
    ~TriangleMesh() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    void ApplyTransform(glm::mat4 transform) override;
    glm::vec3 GetNormal(glm::vec3 hitpos, glm::vec3 rayDir) const override;

    static std::shared_ptr<TriangleMesh> CreateQuad(std::shared_ptr<Material> material);


private:
    std::vector<std::shared_ptr<Triangle>> _triangles;
    BoundingBox _boundingBox;
};
