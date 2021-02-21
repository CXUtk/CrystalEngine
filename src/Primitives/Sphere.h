#pragma once
#include "Object.h"

class Sphere : public Object {
public:
    Sphere(glm::vec3 center, float radius, glm::vec3 rotation);
    ~Sphere() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;

private:
    glm::vec3 _center;
    float _radius;
    glm::mat3 _world2Local;
    glm::mat3 _local2World;
};
