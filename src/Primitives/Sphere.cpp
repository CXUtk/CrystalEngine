#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(glm::vec3 center, float radius) :_center(center), _radius(radius) {
}

Sphere::~Sphere() {
}

BoundingBox Sphere::GetBoundingBox() const {
    return BoundingBox(_center - glm::vec3(abs(_radius)), _center + glm::vec3(abs(_radius)));
}

bool Sphere::Intersect(const Ray& ray, HitRecord* info) const {
    glm::vec3 P = ray.start - _center;
    glm::vec3 d = ray.dir;
    float a = glm::dot(d, d);
    float b = 2 * glm::dot(d, P);
    float c = glm::dot(P, P) - _radius * _radius;
    float discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    discrim = sqrt(discrim);
    float t1 = (-b + discrim) / (2 * a);
    float t2 = (-b - discrim) / (2 * a);
    if (t1 > t2) std::swap(t1, t2);
    float t = t1;
    if (t1 < 0) t = t2;
    if (t < 0) return false;
    info->QuickSetInfo(ray, t, this);
    return true;
}

void Sphere::ApplyTransform(glm::mat4 transform) {
    _center = (transform * glm::vec4(_center, 1));
}
