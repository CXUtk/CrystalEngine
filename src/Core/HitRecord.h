﻿#pragma once
#include <memory>
#include "Utils/Ray.h"
#include "Primitives/Object.h"

class Object;
class HitRecord {
public:
    HitRecord();
    ~HitRecord();

    void SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec2 uv, bool frontFace, const Object* obj, glm::vec3 dpdu, glm::vec3 dpdv);

    void SetUV(glm::vec2 uv) { _uv = uv; }


    bool IsFrontFace() const { return _frontFace; }
    float GetDistance() const { return _distance; }
    const Object* GetHitObject() const { return _hitObject; }
    glm::vec3 GetHitPos() const { return _hitPos; }
    glm::vec3 GetNormal() const { return _normal; }
    glm::vec2 GetUV() const { return _uv; }

private:
    const Object* _hitObject;
    float _distance;
    bool _frontFace;
    glm::vec3 _hitPos, _normal;
    glm::vec2 _uv;
    glm::vec3 _dpdu, _dpdv;
};
