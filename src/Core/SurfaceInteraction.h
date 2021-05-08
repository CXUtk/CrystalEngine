#pragma once
#include <memory>
#include "Utils/Ray.h"
#include <Core/Object.h>
#include <Shapes/Shape.h>

class Object;
class Shape;
class SurfaceInteraction {
public:
    SurfaceInteraction();
    ~SurfaceInteraction();

    Ray SpawnRay(glm::vec3 dir, bool pass = false) const;

    void SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec2 uv,
        bool frontFace, const Shape* shape, glm::vec3 dpdu, glm::vec3 dpdv);

    void SetUV(glm::vec2 uv) { _uv = uv; }
    void SetHitObject(const Object* object) { _hitObject = object; }
    void SetRadianceTransfer(glm::mat3 transferR, glm::mat3 transferG, glm::mat3 transferB) {
        _rTransfer[0] = transferR;
        _rTransfer[1] = transferG;
        _rTransfer[2] = transferB;
    }



    bool IsFrontFace() const { return _frontFace; }
    float GetDistance() const { return _distance; }
    const Shape* GetHitShape() const { return _hitShape; }
    const Object* GetHitObject() const { return _hitObject; }
    glm::vec3 GetHitPos() const { return _hitPos; }
    glm::vec3 GetNormal() const { return _normal; }
    glm::vec2 GetUV() const { return _uv; }
    glm::vec3 GetDpDu() const { return _dpdu; }
    glm::vec3 GetDpDv() const { return _dpdv; }
    glm::mat3 GetRadianceTransfer(int channel) const { return _rTransfer[channel]; }

private:
    const Shape* _hitShape;
    const Object* _hitObject;

    float _distance;
    bool _frontFace;
    glm::vec3 _hitPos, _normal;
    glm::vec2 _uv;
    glm::vec3 _dpdu, _dpdv;
    glm::mat3 _rTransfer[3];
};
