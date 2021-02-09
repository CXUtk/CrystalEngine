#include "Scene.h"
#include "Primitives/Sphere.h"
#include "Primitives/Triangle.h"
#include "Primitives/TriangleMesh.h"
#include "Materials/Default.h"
#include <glm/gtx/transform.hpp>

Scene::Scene() {
    std::shared_ptr<Material> floor = std::make_shared<Default>(glm::vec3(0.5f));
    std::shared_ptr<Material> ball = std::make_shared<Default>(glm::vec3(0.8, 0.5, 0.2f));
    auto sp1 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(0, 1, -1), 1));
    auto sp2 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(-2, -0.5, 2), 1));
    sp1->SetMaterial(ball);
    sp2->SetMaterial(ball);

    _sceneObjects.push_back(sp1);
    _sceneObjects.push_back(sp2);

    glm::vec3 quadVertices[4] = {
        glm::vec3(-25, -2, 45),
        glm::vec3(-25, -2, -5),
        glm::vec3(5, -2, -5),
        glm::vec3(5, -2, 45),
    };

    auto quad = TriangleMesh::CreateQuad(floor);
    glm::mat4 transform = glm::identity<glm::mat4>();
    transform = glm::translate(transform, glm::vec3(-10, -2, 20));
    transform = glm::scale(transform, glm::vec3(30, 1, 50));
    quad->ApplyTransform(transform);
    _sceneObjects.push_back(quad);
}

Scene::~Scene() {
    
}

bool Scene::Intersect(const Ray& ray, HitRecord* hitRecord) const {
    for (auto obj : _sceneObjects) {
        HitRecord tmp;
        if (obj->Intersect(ray, &tmp)) {
            if (tmp.GetDistance() < hitRecord->GetDistance()) {
                memcpy(hitRecord, &tmp, sizeof(HitRecord));
            }
        }
    }
    return hitRecord->GetHitObject() != nullptr;
}
