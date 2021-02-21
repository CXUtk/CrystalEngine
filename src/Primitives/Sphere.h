﻿#pragma once
#include "Object.h"

class Sphere : public Object {
public:
    Sphere(glm::vec3 center, float radius);
    ~Sphere() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    void ApplyTransform(glm::mat4 transform) override;
    glm::vec3 GetNormal(glm::vec3 hitpos, glm::vec3 rayDir) const override;

private:
    glm::vec3 _center;
    float _radius;
};
