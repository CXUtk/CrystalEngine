#include "Cylinder.h"
#include <glm/gtc/matrix_transform.hpp>

Cylinder::Cylinder(glm::vec3 center, float radius, float height, glm::vec3 rotation) : _center(center), _radius(radius), _height(height) {
    glm::mat4 rotMatrix = glm::identity<glm::mat4>();
    rotMatrix = glm::rotate(rotMatrix, rotation.x, glm::vec3(1, 0, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.y, glm::vec3(0, 1, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.z, glm::vec3(0, 0, 1));

    _local2World = glm::mat3(rotMatrix);
    _world2Local = glm::transpose(_local2World);
}

Cylinder::~Cylinder() {
}

BoundingBox Cylinder::GetBoundingBox() const {
    return BoundingBox(glm::vec3(-_radius, -_height / 2, -_radius) + _center,
        glm::vec3(_radius, _height / 2, _radius) + _center);
}



bool Cylinder::Intersect(const Ray& ray, HitRecord* info) const {
    glm::vec3 P = _world2Local * (ray.start - _center);
    glm::vec3 d = _world2Local * ray.dir;

    auto a = (double)d.x * d.x + (double)d.z * d.z;
    auto b = 2.0 * ((double)d.x * P.x + (double)d.z * P.z);
    auto c = ((double)P.x * P.x + (double)P.z * P.z) - (double)_radius * _radius;
    auto discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    auto rootd = std::sqrt(discrim);
    double q;
    if (b < 0) q = -0.5 * (b - rootd);
    else q = -0.5 * (b + rootd);
    float t0 = q / a, t1 = c / q;
    if (t0 > t1) std::swap(t0, t1);
    if (t0 > 0) {
        auto hitpos = P + t0 * d;
        float y = hitpos.y;
        if (y < _height / 2 && y > -_height / 2) {
            setHitInfo(d, t0, hitpos, info);
            return true;
        }
    }
    if (t1 > 0) {
        auto hitpos = P + t1 * d;
        float y = hitpos.y;
        if (y < _height / 2 && y > -_height / 2) {
            setHitInfo(d, t1, hitpos, info);
            return true;
        }
    }
    return false;
}

void Cylinder::setHitInfo(glm::vec3 dir, float t, glm::vec3 localHitPos, HitRecord* info) const {
    auto N = glm::normalize(glm::vec3(localHitPos.x, 0, localHitPos.z));
    auto front_face = glm::dot(dir, N) < 0;
    N = front_face ? N : -N;

    auto realHitPos = _local2World * localHitPos + _center;
    auto theta = std::atan2(-localHitPos.z, localHitPos.x) / glm::pi<float>() + 1.0f;
    auto h = localHitPos.y / _height + 0.5f;
    info->SetHitInfo(t, realHitPos, _local2World * N, glm::vec2(theta, h), front_face, this, glm::vec3(0), glm::vec3(0));
}
