#include "Triangle.h"


Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    _vertices[0] = a, _vertices[1] = b, _vertices[2] = c;
}

Triangle::~Triangle() {
}

BoundingBox Triangle::GetBoundingBox() const {
    glm::vec3 minn = _vertices[0], maxx = _vertices[0];
    for (int i = 1; i < 3; i++) {
        minn.x = std::min(minn.x, _vertices[i].x);
        minn.y = std::min(minn.y, _vertices[i].y);
        maxx.x = std::max(minn.x, _vertices[i].x);
        maxx.y = std::max(minn.y, _vertices[i].y);
    }
    return BoundingBox(minn, maxx);
}

bool Triangle::Intersect(const Ray& ray, HitRecord* info) const {
    glm::mat3 A(_vertices[1] - _vertices[0], _vertices[2] - _vertices[0], -ray.dir);
    auto inv = glm::inverse(A);
    glm::vec3 P = ray.start - _vertices[0];
    auto res = inv * P;
    if (isnan(res.x) || isnan(res.y) || isnan(res.z)) return false;
    if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;

    info->QuickSetInfo(ray, res.z, this);
    info->SetLocalHitInfo(glm::vec3(1-res.x-res.y, res.x, res.y));
    return true;
}

void Triangle::ApplyTransform(glm::mat4 transform) {
    for (int i = 0; i < 3; i++) {
        _vertices[i] = transform * glm::vec4(_vertices[i], 1);
    }
}

glm::vec3 Triangle::GetNormal(glm::vec3 hitpos, glm::vec3 rayDir) const {
    return glm::normalize(glm::cross(_vertices[1] - _vertices[0], _vertices[2] - _vertices[0]));
}
