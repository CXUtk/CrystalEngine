#include "TriangleMesh.h"
#include "Core/GeometryObject.h"
//
//TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 matrix) : _triangles(triangles) {
//
//}

TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 transform,
    const std::shared_ptr<Material>& material, const std::shared_ptr<Light>& light) : _material(material), _light(light) {
    /*transform(triangles, matrix);*/
    _acceleator = Accelerator::GetAccelerator("KDTree");
    transformTriangle(triangles, transform);
    for (auto& tr : triangles) {
        _triangles.push_back(std::make_shared<GeometryObject>(tr, _material, _light));
    }
    _acceleator->Build(_triangles);
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
    return _acceleator->IntersectTest(ray, tMin, tMax);
}

std::shared_ptr<BSDF> TriangleMesh::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    return _material->ComputeScatteringFunctions(isec, fromCamera);
}

void TriangleMesh::transformTriangle(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 transform) {
    auto normalTransfrom = glm::transpose(glm::inverse(transform));
    for (auto& t : triangles) {
        t->ApplyTransform(transform, normalTransfrom);
    }
}
