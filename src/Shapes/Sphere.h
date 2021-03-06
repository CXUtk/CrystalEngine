﻿#pragma once
#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere(glm::vec3 center, float radius, glm::vec3 rotation);
    ~Sphere() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
private:
    glm::vec3 _center;
    float _radius;
    glm::mat3 _world2Local;
    glm::mat3 _local2World;
};
