#pragma once
#include "Object.h"

class Triangle : public Object {
public:
    Triangle() = default;
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    ~Triangle() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    //glm::vec3 GetRealHitPosAndNormal(glm::vec3 hitpos, glm::vec3 rayDir) const override;

private:
    glm::vec3 _vertices[3];
};
