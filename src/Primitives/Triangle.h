#pragma once
#include "Object.h"

class Triangle : public Object {
public:
    Triangle() = default;
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    ~Triangle() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    void ApplyTransform(glm::mat4 transform) override;

private:
    glm::vec3 _vertices[3];
};
