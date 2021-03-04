#include "Scene.h"
#include "Primitives/Sphere.h"
#include "Primitives/Triangle.h"
#include "Primitives/TriangleMesh.h"
#include "Primitives/Cylinder.h"
#include "Materials/Default.h"
#include <Materials/Phong.h>
#include <Materials/Phong_Blinn.h>
#include <Materials/Strauss.h>
#include <Materials/Cook_Torrance.h>
#include <glm/gtx/transform.hpp>
#include <Lights/PointLight.h>
#include <Lights/SphereLight.h>
#include <Materials/Lighted.h>
#include <Materials/Metal.h>
#include <Utils/ObjLoader.h>


Scene::Scene() {
    std::shared_ptr<Material> floor = std::make_shared<Default>(glm::vec3(0.5f), glm::vec2(10, 10));
    std::shared_ptr<Material> ballA = std::make_shared<Default>(glm::vec3(1, 0.1f, 0.1f));
    std::shared_ptr<Material> ballB = std::make_shared<Default>(glm::vec3(1, 1, 0.1f));
    std::shared_ptr<Material> metal = std::make_shared<Metal>(glm::vec3(1, 1, 1), 1.5f);
    std::shared_ptr<Material> lights = std::make_shared<Lighted>(glm::vec3(10, 10, 10));

    std::shared_ptr<Material> ball = std::make_shared<Cook_Torrance>(glm::vec3(0.8, 0.5, 0.2f), 0.2f);

    auto sp1 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(0, 1, -1), 1, glm::vec3(1.f, 0, 0)));
    auto sp2 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(-2, -0.5, 2), 1, glm::vec3(0, 0, 0.5f)));
    sp1->SetMaterial(metal);
    sp2->SetMaterial(ballB);

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

    ObjLoader loader;
    loader.load("Resources/Scenes/cornellbox.obj");
    auto bunny = loader.GetMesh(floor, glm::identity<glm::mat4>());
    //_sceneObjects.push_back(bunny);

    auto cylinder = std::shared_ptr<Cylinder>(new Cylinder(glm::vec3(2, 1.f, 2), 1, 1, glm::vec3(1.f, 0.f, 0.5f)));
    cylinder->SetMaterial(ballB);
    _sceneObjects.push_back(cylinder);



    auto light1 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(0, 5, 0), 0.8, glm::vec3(1.f, 0, 0)));
    auto light2 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(5, 3, -5), 0.8, glm::vec3(1.f, 0, 0)));
    light1->SetMaterial(lights);
    light2->SetMaterial(lights);
    _sceneObjects.push_back(light1);
    _sceneObjects.push_back(light2);

    AddLight(std::make_shared<SphereLight>(glm::vec3(0, 5, 0), 1, glm::vec3(1), 1000));
    AddLight(std::make_shared<SphereLight>(glm::vec3(5, 3, -5), 1, glm::vec3(1), 600));
    //AddLight(std::make_shared<PointLight>(glm::vec3(5, 5, -5), glm::vec3(1), 200));
    //AddLight(std::make_shared<PointLight>(glm::vec3(0, 0, 0), glm::vec3(1), 20));

    _accelerator = Accelerator::GetAccelerator("KDTree");
    _accelerator->Build(_sceneObjects);
}

Scene::~Scene() {

}

bool Scene::Intersect(const Ray& ray, HitRecord* hitRecord) const {
    return _accelerator->Intersect(ray, hitRecord);
}

bool Scene::IntersectWithLight(const Ray& ray, HitRecord* hitRecord) const {
    bool hit = _accelerator->Intersect(ray, hitRecord);
    return false;
}

void Scene::AddObject(std::shared_ptr<Object> object) {
    _sceneObjects.push_back(object);
}

void Scene::AddLight(std::shared_ptr<Light> light) {
    _lights.push_back(light);
}
