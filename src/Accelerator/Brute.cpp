//#include "Brute.h"
//
//Brute::Brute() {
//}
//
//Brute::~Brute() {
//}
//
//void Brute::Build(const std::vector<std::shared_ptr<Object>>& objects) {
//    for (auto &ptr : objects) {
//        _objects.push_back(ptr.get());
//    }
//}
//
//bool Brute::Intersect(const Ray& ray, SurfaceInteraction* isec) const {
//    for (auto obj : _objects) {
//        SurfaceInteraction tmp;
//        if (obj->Intersect(ray, &tmp)) {
//            if (tmp.GetDistance() < isec->GetDistance()) {
//                memcpy(isec, &tmp, sizeof(isec));
//            }
//        }
//    }
//    return isec->GetHitShape() != nullptr;
//}
