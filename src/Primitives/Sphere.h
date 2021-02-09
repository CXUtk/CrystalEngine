#pragma once
#include "Object.h"

class Sphere : public Object {
public:
    Sphere(glm::vec3 center, float radius);
    ~Sphere() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    void ApplyTransform(glm::mat4 transform) override;

private:
    glm::vec3 _center;
    float _radius;
};
