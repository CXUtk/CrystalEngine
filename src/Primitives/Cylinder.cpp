#include "Cylinder.h"

Cylinder::Cylinder(glm::vec3 center, float radius, float height) :_center(center), _radius(radius), _height(height) {
}

Cylinder::~Cylinder() {
}

BoundingBox Cylinder::GetBoundingBox() const {
    return BoundingBox(glm::vec3(-_radius, -_height / 2, -_radius) + _center,
        glm::vec3(_radius, _height / 2, _radius) + _center);
}

bool Cylinder::Intersect(const Ray& ray, HitRecord* info) const {
    auto rayStart = ray.start - _center;
    auto a = (double)ray.dir.x * ray.dir.x + (double)ray.dir.z * ray.dir.z;
    auto b = 2.0 * ((double)ray.dir.x * rayStart.x + (double)ray.dir.z * rayStart.z);
    auto c = ((double)rayStart.x * rayStart.x + (double)rayStart.z * rayStart.z) - (double)_radius * _radius;
    auto discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    auto rootd = std::sqrt(discrim);
    double q;
    if (b < 0) q = -0.5 * (b - rootd);
    else q = -0.5 * (b + rootd);
    float t0 = q / a, t1 = c / q;
    if (t0 > t1) std::swap(t0, t1);
    if (t0 > 0) {
        float z = (rayStart + t0 * ray.dir).y;
        if (z <= _height / 2 && z >= -_height / 2) {
            info->QuickSetInfo(ray, t0, this);
            return true;
        }
    }
    if (t1 > 0) {
        float z = (rayStart + t1 * ray.dir).y;
        if (z <= _height / 2 && z >= -_height / 2) {
            info->QuickSetInfo(ray, t1, this);
            return true;
        }
    }
    return false;
}

void Cylinder::ApplyTransform(glm::mat4 transform) {
}

glm::vec3 Cylinder::GetNormal(glm::vec3 hitpos, glm::vec3 rayDir) const {
    return glm::vec3();
}
