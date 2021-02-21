#pragma once
#include <Core/HitRecord.h>
class Light {
public:
    Light() {}
    virtual ~Light() = 0 {}
    virtual glm::vec3 SampleLi(const HitRecord& hit, glm::vec3& endpoint) = 0 {}
};