#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

enum BxDFType {
    BxDF_REFLECTION = 1 << 0,
    BxDF_TRANSMISSION = 1 << 1,
    BxDF_GLOSSY = 1 << 2,
    BxDF_SPECULAR = 1 << 3,
    BxDF_DIFFUSE = 1 << 4,
    BxDF_ALL = 0x1F,
};

class BxDF;
class SurfaceInteraction;

class BSDF {
public:
    BSDF(const SurfaceInteraction* si);
    int NumBxDFs() const { return _bxdfs.size(); }
    void AddBxDF(const std::shared_ptr<BxDF>& bxdf, glm::vec3 w);
    
    glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const;
    glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType sampleType) const;

private:
    const SurfaceInteraction* _hit;
    std::vector<glm::vec3> _weights;
    std::vector<std::shared_ptr<BxDF>> _bxdfs;
};

class BxDF {
public:
    BxDF(BxDFType type) : _bxdfType(type), _active(true) {}
    virtual ~BxDF() = 0 {}

    virtual glm::vec3 DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const = 0;
    virtual glm::vec3 SampleDirection(glm::vec3 wOut, glm::vec3* wIn, float* pdf) const = 0;

    bool Contains(BxDFType type) const { return (_bxdfType & type); }
    
    bool IsActive() const { return _active; }
    void SetActive(bool value) { _active = value; }

private:
    BxDFType _bxdfType;
    bool _active;
};