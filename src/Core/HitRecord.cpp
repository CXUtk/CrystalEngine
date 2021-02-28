#include "HitRecord.h"

HitRecord::HitRecord() :_hitObject(nullptr),
_distance(std::numeric_limits<float>::infinity()), 
_hitPos(0), _uv(0), _normal(0), _dpdu(0), _dpdv(0),
_frontFace(false) {

}

HitRecord::~HitRecord() {
}

void HitRecord::SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec2 uv, bool frontFace, const Object* obj, glm::vec3 dpdu, glm::vec3 dpdv) {
    _hitPos = hitPos;
    _distance = t;
    _hitObject = obj;
    _uv = uv;
    _normal = normal;
    _frontFace = frontFace;
    _dpdu = dpdu;
    _dpdv = dpdv;
}


//void HitRecord::set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
//    //auto front_face = glm::dot(r.dir, outward_normal) < 0;
//    //_normal = front_face ? outward_normal : -outward_normal;
//    //_frontFace = front_face;
//}
