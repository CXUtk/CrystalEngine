﻿#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 rotation) :_center(center), _radius(radius) {
    glm::mat4 rotMatrix = glm::identity<glm::mat4>();
    rotMatrix = glm::rotate(rotMatrix, rotation.x, glm::vec3(1, 0, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.y, glm::vec3(0, 1, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.z, glm::vec3(0, 0, 1));

    _local2World = glm::mat3(rotMatrix);
    _world2Local = glm::transpose(_local2World);
}

Sphere::~Sphere() {
}

BoundingBox Sphere::GetBoundingBox() const {
    return BoundingBox(_center - glm::vec3(abs(_radius)), _center + glm::vec3(abs(_radius)));
}

bool Sphere::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    glm::vec3 P = _world2Local * (ray.start - _center);
    glm::vec3 d = _world2Local * ray.dir;
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

    // Calculate local hit info, normal, front face, etc..
    auto dummyHitPos = P + d * t;
    auto N = glm::normalize(dummyHitPos);
    auto theta = std::atan2(-dummyHitPos.z, dummyHitPos.x) / glm::pi<float>() + 1.0f;
    auto phi = std::acos(dummyHitPos.y / _radius);

    auto front_face = glm::dot(d, N) < 0;
    N = front_face ? N : -N;

    auto realHitPos = _local2World * dummyHitPos + _center;
    info->SetHitInfo(t, realHitPos, _local2World * N, glm::vec2(theta, phi), front_face, this,
        _local2World * glm::vec3(-dummyHitPos.z, 0, dummyHitPos.x),
        _local2World * glm::vec3(0));

    //if (glm::isnan(_local2World * dummyHitPos) != glm::bvec3(0)) {
    //    printf("%lf %lf %lf %lf %lf\n", ray.dir.x, ray.dir.y, ray.dir.z, a, t2);
    //    fflush(stdout);
    //    return false;
    //}
    return true;
}

bool Sphere::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    if (tMin > tMax) return false;

    glm::vec3 P = _world2Local * (ray.start - _center);
    glm::vec3 d = _world2Local * ray.dir;
    float a = glm::dot(d, d);
    float b = 2 * glm::dot(d, P);
    float c = glm::dot(P, P) - _radius * _radius;
    float discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    discrim = sqrt(discrim);
    float t1 = (-b + discrim) / (2 * a);
    float t2 = (-b - discrim) / (2 * a);
    return (t1 >= tMin && t1 <= tMax) || (t2 >= tMin && t2 <= tMax);
}
