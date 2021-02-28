#pragma once
#include "Object.h"
#include "Primitives.h"
class Triangle : public Object {
public:
    Triangle() = default;
    Triangle(const VertexData& a, const VertexData& b, const VertexData& c);
    ~Triangle() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;
    //glm::vec3 GetRealHitPosAndNormal(glm::vec3 hitpos, glm::vec3 rayDir) const override;

    void ApplyTransform(glm::mat4 transform, glm::mat4 normalTransform);

private:
    VertexData _vertices[3];
};
