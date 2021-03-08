#pragma once
#include <glm/glm.hpp>
enum BxDFType {
    BxDF_REFLECTION = 1 << 0,
    BxDF_TRANSMISSION = 1 << 1,
    BxDF_GLOSSY = 1 << 2,
    BxDF_SPECULAR = 1 << 3,
    BxDF_DIFFUSE = 1 << 4,
    BxDF_ALL = 0x1F,
};

class BSDF {
public:
    BSDF(BxDFType type) : _bxdfType(type), _active(true) {}
    virtual ~BSDF() = 0 {}

    virtual glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const = 0;
    virtual glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const = 0;

    bool Contains(BxDFType type) const { return (_bxdfType & type) == type; }
    
    bool IsActive() const { return _active; }
    void SetActive(bool value) { _active = value; }

private:
    BxDFType _bxdfType;
    bool _active;
};