#include "Scene.h"
#include "Materials/Default.h"
#include <glm/gtx/transform.hpp>
#include <Lights/PointLight.h>
#include <Lights/AreaLight.h>
#include <Lights/SphereLight.h>
#include <Materials/Metal.h>
#include <Utils/ObjLoader.h>
#include <Shapes/TriangleMesh.h>
#include <Core/GeometryObject.h>
#include <Shapes/Sphere.h>
#include <Materials/Glass.h>
#include <Materials/Phong.h>
#include <Materials/Brick.h>
#include <Textures/ImageTexture.h>
#include <lodepng/lodepng.h>

//#include <Materials/Phong_Blinn.h>
//#include <Materials/Strauss.h>
//#include <Materials/Cook_Torrance.h>
//#include <Materials/Lighted.h>


std::shared_ptr<TriangleMesh> CreateQuad(glm::mat4 transform, const std::shared_ptr<Material>& material, const std::shared_ptr<Light>& light = nullptr) {
    static VertexData quadVertices[4] = {
        { glm::vec3(-0.5, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0)  },
        { glm::vec3(-0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
        { glm::vec3(0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
        { glm::vec3(0.5, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0)  },
    };
    std::vector<std::shared_ptr<Triangle>> list;
    std::shared_ptr<Triangle> tr1 = std::shared_ptr<Triangle>(new Triangle(quadVertices[0], quadVertices[1], quadVertices[2]));
    std::shared_ptr<Triangle> tr2 = std::shared_ptr<Triangle>(new Triangle(quadVertices[0], quadVertices[2], quadVertices[3]));

    list.push_back(tr1);
    list.push_back(tr2);
    return std::make_shared<TriangleMesh>(list, transform, material, light);
}


std::vector<std::shared_ptr<Object>> CreateCornellBox() {
    std::vector<std::shared_ptr<Object>> objs;

    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 transform = identity;

    transform = glm::translate(transform, glm::vec3(0, 0, -3));
    transform = glm::scale(transform, glm::vec3(6, 1, 6));



    std::shared_ptr<Material> groundMat = std::make_shared<Brick>(glm::vec3(0.6, 0.15, 0.1), glm::vec3(1), 0.09f, 0.04f, 0.01f);
    std::shared_ptr<Material> ceilMat = std::make_shared<Default>(glm::vec3(1.f), glm::vec2(0));
    auto ground = CreateQuad(transform, ceilMat);
    transform = identity;
    transform = glm::translate(transform, glm::vec3(0, 6, -3));
    transform = glm::scale(transform, glm::vec3(6, 1, 6));
    auto ceil = CreateQuad(transform, ceilMat);


    transform = identity;
    transform = glm::translate(transform, glm::vec3(-3, 3, -3));
    transform = glm::rotate_slow(transform, glm::half_pi<float>(), glm::vec3(0, 0, 1));
    transform = glm::scale(transform, glm::vec3(6, 1, 6));
    std::shared_ptr<Material> leftWallMat = std::make_shared<Default>(glm::vec3(1, 0.1f, 0.1f), glm::vec2(0, 0));
    auto leftWall = CreateQuad(transform, leftWallMat);

    transform = identity;
    transform = glm::translate(transform, glm::vec3(3, 3, -3));
    transform = glm::rotate_slow(transform, glm::half_pi<float>(), glm::vec3(0, 0, 1));
    transform = glm::scale(transform, glm::vec3(6, 1, 6));
    std::shared_ptr<Material> rightWallMat = std::make_shared<Default>(glm::vec3(0.1f, 1, 0.1f), glm::vec2(0, 0));
    auto rightWall = CreateQuad(transform, rightWallMat);

    transform = identity;
    transform = glm::translate(transform, glm::vec3(0, 3, -6));
    transform = glm::rotate_slow(transform, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    transform = glm::scale(transform, glm::vec3(6, 1, 6));
    auto texture = std::make_shared<ImageTexture>("Resources/Textures/twist.png");
    //std::shared_ptr<Material> farWallMat = std::make_shared<Phong>(texture);
    std::shared_ptr<Material> farWallMat = std::make_shared<Default>(glm::vec3(1.0f, 1.0f, 0.f));
    auto farWall = CreateQuad(transform, farWallMat);

    objs.push_back(ground);
    objs.push_back(ceil);
    objs.push_back(leftWall);
    objs.push_back(rightWall);
    objs.push_back(farWall);
    return objs;
}


Scene::Scene() {
    std::shared_ptr<Material> floor = std::make_shared<Default>(glm::vec3(0.5f), glm::vec2(10, 10));
    for (auto& obj : CreateCornellBox()) {
        AddObject(obj);
    }
    std::shared_ptr<Material> ballA = std::make_shared<Default>(glm::vec3(1.0f));
    std::shared_ptr<Material> ballB = std::make_shared<Phong>(glm::vec3(0.1f, 0.2f, 0.7f));
    std::shared_ptr<Material> ballC = std::make_shared<Phong>(glm::vec3(1.f, 1.f, 1.f));
    std::shared_ptr<Material> ballD = std::make_shared<Default>(glm::vec3(1.0f, 1.0f, 1.0f));
    std::shared_ptr<Material> reflectMat = std::make_shared<Metal>(glm::vec3(.8f, 0.8f, 0.2f), 1.0f);
    std::shared_ptr<Material> glassMat = std::make_shared<Glass>(glm::vec3(1.0f), 1.5f);
    //std::shared_ptr<Material> ballB = std::make_shared<Metal>(glm::vec3(1, 1, 1), 2.f);
    //std::shared_ptr<Material> metal = std::make_shared<Phong>(glm::vec3(1, 0.5f, 0.5f), 128, glm::vec3(0.1f, 0.5f, 1.0f));
    ////std::shared_ptr<Material> lights = std::make_shared<Lighted>(glm::vec3(10, 10, 10));

    ////std::shared_ptr<Material> ball = std::make_shared<Cook_Torrance>(glm::vec3(0.8, 0.5, 0.2f), 0.2f);

    auto sp1 = std::make_shared<GeometryObject>(std::make_shared<Sphere>(glm::vec3(0, 3, -1), 0.9, glm::vec3(0)), glassMat, nullptr);
    auto sp2 = std::make_shared<GeometryObject>(std::make_shared<Sphere>(glm::vec3(-2, 1, -3), 0.9, glm::vec3(0)), ballD, nullptr);
    auto sp3 = std::make_shared<GeometryObject>(std::make_shared<Sphere>(glm::vec3(0, 1, -2), 0.5, glm::vec3(0)), ballD, nullptr);
    auto sp4 = std::make_shared<GeometryObject>(std::make_shared<Sphere>(glm::vec3(2, 1, -3), 0.9, glm::vec3(0)), ballC, nullptr);

    AddObject(sp1);
    //AddObject(sp2);
    //AddObject(sp3);
    AddObject(sp4);
    //_sceneObjects.push_back(sp1);
    //_sceneObjects.push_back(sp2);





    ObjLoader loader;
    loader.load("Resources/Scenes/cube.obj");
    auto transform = glm::identity<glm::mat4>();
    transform = glm::translate(transform, glm::vec3(-1, 0.2, -3));
    auto bunny = loader.GetMesh(ballD, transform);
    AddObject(bunny);

    //transform = glm::identity<glm::mat4>();
    //transform = glm::translate(transform, glm::vec3(1, 1, -1));
    //bunny = loader.GetMesh(ballD, transform);
    //AddObject(bunny);

    //auto cylinder = std::shared_ptr<Cylinder>(new Cylinder(glm::vec3(2, -1.f, 2), 1, 1, glm::vec3(0.f, 0.f, 0.f)));
    //cylinder->SetMaterial(metal);
    //_sceneObjects.push_back(cylinder);



    //auto light1 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(0, 5, 0), 0.8, glm::vec3(1.f, 0, 0)));
    //auto light2 = std::shared_ptr<Sphere>(new Sphere(glm::vec3(5, 3, -5), 0.8, glm::vec3(1.f, 0, 0)));
    //light1->SetMaterial(lights);
    //light2->SetMaterial(lights);
    //_sceneObjects.push_back(light1);
    //_sceneObjects.push_back(light2);

    //AddLight(std::make_shared<SphereLight>(glm::vec3(0, 5, 0), 1, glm::vec3(1), 200));
    //AddLight(std::make_shared<SphereLight>(glm::vec3(5, 3, -5), 1, glm::vec3(1), 200));
    //AddLight(std::make_shared<PointLight>(glm::vec3(5, 5, -5), glm::vec3(1), 20));
    //AddLight(std::make_shared<PointLight>(glm::vec3(2, 3, 3), glm::vec3(1), 30));
    //AddLight(std::make_shared<PointLight>(glm::vec3(0, 3, 3), glm::vec3(1), 12));

    auto areaLight = std::make_shared<AreaLight>(glm::vec3(1, 5.99, -2), glm::vec3(-2, 0, 0), glm::vec3(0, 0, -2), 10);
    AddLight(areaLight);

    glm::mat4 identity = glm::identity<glm::mat4>();
    transform = identity;

    transform = glm::translate(transform, glm::vec3(0, 5.995, -3));
    transform = glm::scale(transform, glm::vec3(2, 1, 2));

    auto light = CreateQuad(transform, nullptr, areaLight);
    AddObject(light);

    _accelerator = Accelerator::GetAccelerator("KDTree");
    _accelerator->Build(_sceneObjects);
}

Scene::~Scene() {

}

bool Scene::Intersect(const Ray& ray, SurfaceInteraction* isec) const {
    return _accelerator->Intersect(ray, isec);
}

bool Scene::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    return _accelerator->IntersectTest(ray, tMin, tMax);
}

bool Scene::IntersectWithLight(const Ray& ray, SurfaceInteraction* isec) const {
    bool hit = _accelerator->Intersect(ray, isec);
    return false;
}

void Scene::AddObject(std::shared_ptr<Object> object) {
    _sceneObjects.push_back(object);
}

void Scene::AddLight(std::shared_ptr<Light> light) {
    _lights.push_back(light);
}
