#pragma once
#include <memory>
#include "Utils/Ray.h"
#include "Primitives/Object.h"

class Object;
class HitRecord {
public:
    HitRecord();
    ~HitRecord();

    void SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec3 localInfo, bool frontFace, const Object* obj);

    void SetLocalHitInfo(glm::vec3 info) { _localHitInfo = info; }


    bool IsFrontFace() const { return _frontFace; }
    float GetDistance() const { return _distance; }
    const Object* GetHitObject() const { return _hitObject; }
    glm::vec3 GetHitPos() const { return _hitPos; }
    glm::vec3 GetNormal() const { return _normal; }
    glm::vec3 GetLocalHitInfo() const { return _localHitInfo; }

private:
    const Object* _hitObject;
    float _distance;
    bool _frontFace;
    glm::vec3 _hitPos;
    glm::vec3 _localHitInfo;
    glm::vec3 _normal;
};
