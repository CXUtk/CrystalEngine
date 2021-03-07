#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 matrix) : _triangles(triangles) {
    transform(matrix);
    _acceleator = Accelerator::GetAccelerator("KDTree");
    std::vector<std::shared_ptr<Object>> objects(_triangles.begin(), _triangles.end());
    _acceleator->Build(objects);
}

TriangleMesh::~TriangleMesh() {
}

BoundingBox TriangleMesh::GetBoundingBox() const {
    BoundingBox box;
    for (auto& tri : _triangles) {
        box = box.Union(tri->GetBoundingBox());
    }
    return box;
}

bool TriangleMesh::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    return _acceleator->Intersect(ray, info);
}

bool TriangleMesh::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    SurfaceInteraction tmp;
    return _acceleator->Intersect(ray, &tmp);
}

void TriangleMesh::transform(glm::mat4 transform) {
    auto normalTransfrom = glm::transpose(glm::inverse(transform));
    for (auto& t : _triangles) {
        t->ApplyTransform(transform, normalTransfrom);
    }
}
