#include "HitRecord.h"

HitRecord::HitRecord() :_hitObject(nullptr),
_distance(std::numeric_limits<float>::infinity()), 
_hitPos(0), _localHitInfo(0), 
_normal(0),
_frontFace(false) {

}

HitRecord::~HitRecord() {
}

void HitRecord::SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec3 localInfo, bool frontFace, const Object* obj) {
    if (t >= _distance) return;
    _hitPos = hitPos;
    _distance = t;
    _hitObject = obj;
    _localHitInfo = localInfo;
    _normal = normal;
    _frontFace = frontFace;
}


//void HitRecord::set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
//    //auto front_face = glm::dot(r.dir, outward_normal) < 0;
//    //_normal = front_face ? outward_normal : -outward_normal;
//    //_frontFace = front_face;
//}
