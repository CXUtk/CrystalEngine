#pragma once
#include <vector>
#include <memory>
#include <Core/SurfaceInteraction.h>
#include <Utils/Ray.h>
#include <Core/Object.h>
#include <Lights/Light.h>
#include <Accelerator/Accelerator.h>
#include <Shapes/TriangleMesh.h>

class Scene {
public:
    Scene();
    ~Scene();

    bool Intersect(const Ray& ray, SurfaceInteraction* SurfaceInteraction) const;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const;
    bool IntersectWithLight(const Ray& ray, SurfaceInteraction* SurfaceInteraction) const;

    void AddObject(std::shared_ptr<Object> object);
    void AddLight(std::shared_ptr<Light> light);
    void AddTriangleMesh(std::shared_ptr<TriangleMesh> mesh);

    std::vector<std::shared_ptr<Light>> GetLights() const { return _lights; }
    std::vector<std::shared_ptr<TriangleMesh>> GetTriangleMeshes() const { return _triangleMeshes; }



private:
    std::vector<std::shared_ptr<Object>> _sceneObjects;
    std::vector<std::shared_ptr<Light>> _lights;
    std::vector<std::shared_ptr<TriangleMesh>> _triangleMeshes;

    std::unique_ptr<Accelerator> _accelerator;

    void buildScene1();
    void buildScene2();
};
