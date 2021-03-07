#pragma once
#include "Shape.h"

class Cylinder : public Shape {
public:
    Cylinder(glm::vec3 center, float radius, float height, glm::vec3 rotation);
    ~Cylinder() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

private:
    glm::vec3 _center;
    float _radius;
    float _height;
    glm::mat3 _world2Local;
    glm::mat3 _local2World;

    void setHitInfo(glm::vec3 dir, float t, glm::vec3 localHitPos, SurfaceInteraction* info) const;
};
