#pragma once
#include "Object.h"

class Cylinder : public Object {
public:
    Cylinder(glm::vec3 center, float radius, float height);
    ~Cylinder() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    void ApplyTransform(glm::mat4 transform) override;

private:
    glm::vec3 _center;
    float _radius;
    float _height;
};
