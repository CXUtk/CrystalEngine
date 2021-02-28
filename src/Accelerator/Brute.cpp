#include "Brute.h"

Brute::Brute() {
}

Brute::~Brute() {
}

void Brute::Build(const std::vector<std::shared_ptr<Object>>& objects) {
    for (auto ptr : objects) {
        _objects.push_back(ptr.get());
    }
}

bool Brute::Intersect(const Ray& ray, HitRecord* hitRecord) const {
    for (auto obj : _objects) {
        HitRecord tmp;
        if (obj->Intersect(ray, &tmp)) {
            if (tmp.GetDistance() < hitRecord->GetDistance()) {
                memcpy(hitRecord, &tmp, sizeof(HitRecord));
            }
        }
    }
    return hitRecord->GetHitObject() != nullptr;
}
