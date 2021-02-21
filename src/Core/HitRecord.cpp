#include "HitRecord.h"

HitRecord::HitRecord() :_hitObject(nullptr),
_distance(std::numeric_limits<float>::infinity()), 
_hitPos(0), _localHitInfo(0), 
_normal(0),
_frontFace(false) {

}

HitRecord::~HitRecord() {
}

void HitRecord::QuickSetInfo(const Ray& ray, float t, const Object* obj) {
    if (t >= _distance) return;
    auto hitpos = t * ray.dir + ray.start;
    _hitPos = hitpos;
    _distance = t;
    _hitObject = obj;
    set_face_normal(ray, obj->GetNormal(hitpos, ray.dir));
}

void HitRecord::set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
    auto front_face = glm::dot(r.dir, outward_normal) < 0;
    _normal = front_face ? outward_normal : -outward_normal;
    _frontFace = front_face;
}
