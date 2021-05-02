#include "GeometryObject.h"

GeometryObject::GeometryObject(const std::shared_ptr<Shape>& shape,
    const std::shared_ptr<Material>& material,
    const std::shared_ptr<Light>& light) : _shape(shape), _material(material), _light(light) {

}

bool GeometryObject::Intersect(const Ray& ray, SurfaceInteraction* isec) const {
    bool hit = _shape->Intersect(ray, isec);
    if (!hit) return false;
    isec->SetHitObject(this);
    return hit;
}

std::shared_ptr<BSDF> GeometryObject::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    return _material->ComputeScatteringFunctions(isec, fromCamera);
}
