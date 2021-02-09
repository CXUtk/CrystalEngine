#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include "Ray.h"

// Represents the bounding box of a geometry object
class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);
    ~BoundingBox();

    glm::vec3 getMinPos()const { return _minPos; }
    glm::vec3 getMaxPos()const { return _maxPos; }

    bool rayIntersect(const Ray& ray, float& tMin, float& tMax) const;

    bool intersects(const BoundingBox& box) const;

    glm::vec3 getCenter() const { return (_minPos + _maxPos) / 2.f; }

    BoundingBox unionWith(const BoundingBox& box) const;

    BoundingBox unionWith(const glm::vec3& pos) const;

    BoundingBox intersectWith(const BoundingBox& box) const;

    int MaxExtent() const;

    float SurfaceArea() const;

private:
    glm::vec3 _minPos, _maxPos;
};
