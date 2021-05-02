﻿#pragma once
#include "Shape.h"
#include "Primitives.h"
class Scene;
class Triangle : public Shape {
public:
    Triangle(const VertexData& a, const VertexData& b, const VertexData& c);
    ~Triangle() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

    void ApplyTransform(glm::mat4 transform, glm::mat4 normalTransform);

    void ComputeTransferFunction(const std::shared_ptr<Scene>& scene);

private:
    VertexData _vertices[3];
    glm::vec3 _dpdu, _dpdv;

    void calculateDerivative();
};
