#include "Scene.h"
#include "Primitives/Sphere.h"
#include "Primitives/Triangle.h"
#include "Primitives/TriangleMesh.h"
#include "Primitives/Cylinder.h"
#include "Materials/Default.h"
#include <Materials/Phong.h>
#include <Materials/Phong_Blinn.h>
#include <Materials/Strauss.h>
#include <glm/gtx/transform.hpp>
#include <Lights/PointLight.h>
#include <Materials/Cook_Torrance.h>

Scene::Scene() {
    std::shared_ptr<Material> floor = std::make_shared<Phong>(glm::vec3(0.5f), 64, glm::vec3(1));

    std::shared_ptr<Material> ball = std::make_shared<Cook_Torrance>(glm::vec3(0.8, 0.5, 0.2f), 0.2f);

    auto sp1 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(0, 1, -1), 1, glm::vec3(1.f, 0, 0)));
    auto sp2 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(-2, -0.5, 2), 1, glm::vec3(0, 0, 0.5f)));
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

    glm::mat4 transform = glm::identity<glm::mat4>();
    transform = glm::translate(transform, glm::vec3(-10, -2, 20));
    transform = glm::scale(transform, glm::vec3(30, 1, 50));
    auto quad = TriangleMesh::CreateQuad(floor, transform);
    _sceneObjects.push_back(quad);

    //auto cylinder = std::shared_ptr<Cylinder>(new Cylinder(glm::vec3(0, 1, 0), 1, 1, glm::vec3(-0.5f, 0.f, 0.5f)));
    //cylinder->SetMaterial(ball);
    //_sceneObjects.push_back(cylinder);


    AddLight(std::make_shared<PointLight>(glm::vec3(7, 15, -15), glm::vec3(1), 600));
    AddLight(std::make_shared<PointLight>(glm::vec3(-7, 15, -15), glm::vec3(1), 600));
    //AddLight(std::make_shared<PointLight>(glm::vec3(0, 0, 0), glm::vec3(1), 20));

    _accelerator = Accelerator::GetAccelerator("KDTree");
    _accelerator->Build(_sceneObjects);
}

Scene::~Scene() {

}

bool Scene::Intersect(const Ray& ray, HitRecord* hitRecord) const {
    return _accelerator->Intersect(ray, hitRecord);
}

void Scene::AddObject(std::shared_ptr<Object> object) {
    _sceneObjects.push_back(object);
}

void Scene::AddLight(std::shared_ptr<Light> light) {
    _lights.push_back(light);
}
