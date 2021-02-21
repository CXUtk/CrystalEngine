#pragma once
#include "Object.h"

class Cylinder : public Object {
public:
    Cylinder(glm::vec3 center, float radius, float height, glm::vec3 rotation);
    ~Cylinder() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;

private:
    glm::vec3 _center;
    float _radius;
    float _height;
    glm::mat3 _world2Local;
    glm::mat3 _local2World;

    void setHitInfo(glm::vec3 dir, float t, glm::vec3 localHitPos, HitRecord* info) const;
};
