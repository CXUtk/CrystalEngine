#include "TriangleMesh.h"
#include "Core/GeometryObject.h"
#include <Core/Scene.h>

TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 transform,
    const std::shared_ptr<Material>& material, const std::shared_ptr<Light>& light) : _material(material), _light(light) {
    /*transform(triangles, matrix);*/
    _acceleator = Accelerator::GetAccelerator("KDTree");
    transformTriangle(triangles, transform);
    std::vector<std::shared_ptr<Object>> objs;
    for (auto& tr : triangles) {
        auto triangle = std::make_shared<GeometryObject>(tr, _material, _light);
        _triangles.push_back(triangle);
        objs.push_back(triangle);
    }
    _acceleator->Build(objs);
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

void TriangleMesh::PrecomputeRadianceTransfer(const std::shared_ptr<Scene>& scene) {
    for (auto& t : _triangles) {
        auto triangle = dynamic_cast<Triangle*>(t->GetShape().get());
        triangle->ComputeTransferFunction(scene);
    }
}


void TriangleMesh::transformTriangle(const std::vector<std::shared_ptr<Triangle>>& triangles, glm::mat4 transform) {
    auto normalTransfrom = glm::transpose(glm::inverse(transform));
    for (auto& t : triangles) {
        t->ApplyTransform(transform, normalTransfrom);
    }
}
