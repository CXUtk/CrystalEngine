#pragma once
#include <Core/Object.h>
#include "Triangle.h"
#include <Accelerator/Accelerator.h>
#include <vector>


class Scene;
class GeometryObject;
class TriangleMesh : public Object {
public:
    TriangleMesh(const std::vector<VertexData>& Vertices, const std::vector<glm::ivec3>& triangleFaceIndices, const std::shared_ptr<Material>& material,
        const glm::mat4 transform,
        const std::shared_ptr<Light>& light);
    ~TriangleMesh() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

    const Light* GetLight() const override { return _light.get(); }
    const Material* GetMaterial() const override { return _material.get(); }
    std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const override;

    void ComputeInitialRadianceTransfer(const std::shared_ptr<Scene>& scene);
    void ComputeInterReflection(const std::shared_ptr<Scene>& scene);
    void WritePRTInfo(FILE* file);

    void SetMaterial(const std::shared_ptr<Material>& material) { _material = material; }

    //static std::shared_ptr<TriangleMesh> CreateQuad(std::shared_ptr<Material> material, glm::mat4 transform);
private:
    std::vector<std::shared_ptr<GeometryObject>> _triangles;
    BoundingBox _boundingBox;
    std::unique_ptr<Accelerator> _acceleator;

    std::shared_ptr<Material> _material;
    std::shared_ptr<Light> _light;
    std::vector<VertexData> _vertices;

    glm::mat4 _transform;
};
