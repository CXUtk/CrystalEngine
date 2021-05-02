#include "TriangleMesh.h"
#include "Core/GeometryObject.h"
#include <Core/Scene.h>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
#include <PRTEvaluator/SHEval.h>
#include <thread>



TriangleMesh::TriangleMesh(const std::vector<VertexData>& Vertices, const std::vector<glm::ivec3>& triangleFaceIndices, const std::shared_ptr<Material>& material,
    const std::shared_ptr<Light>& light) : _vertices(Vertices), _material(material), _light(light) {
    _acceleator = Accelerator::GetAccelerator("KDTree");

    int faceCount = triangleFaceIndices.size();
    std::vector<std::shared_ptr<Object>> objs;
    for (int i = 0; i < faceCount; i++) {
        auto& t = triangleFaceIndices[i];
        auto triangle = std::make_shared<Triangle>(&_vertices[t.x], &_vertices[t.y], &_vertices[t.z]);
        auto triangleObject = std::make_shared<GeometryObject>(triangle, _material, _light);
        _triangles.push_back(triangleObject);
        objs.push_back(triangleObject);
    }

    //transformTriangle(triangles, transform);

    //for (auto& tr : triangles) {
    //    auto triangle = std::make_shared<GeometryObject>(tr, _material, _light);
    //    _triangles.push_back(triangle);
    //    objs.push_back(triangle);
    //}
    _acceleator->Build(objs);
}

TriangleMesh::~TriangleMesh() {
}

BoundingBox TriangleMesh::GetBoundingBox() const {
    BoundingBox box;
    for (auto& tri : _triangles) {
        box = box.Union(tri->GetBoundingBox());
    }
    return box;
}

bool TriangleMesh::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    return _acceleator->Intersect(ray, info);
}

bool TriangleMesh::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    return _acceleator->IntersectTest(ray, tMin, tMax);
}

std::shared_ptr<BSDF> TriangleMesh::ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera) const {
    return _material->ComputeScatteringFunctions(isec, fromCamera);
}

void TriangleMesh::PrecomputeRadianceTransfer(const std::shared_ptr<Scene>& scene) {
    constexpr int NUM_SAMPLES = 200;
    int BLOCK = std::sqrt(NUM_SAMPLES / 2);
    float step = glm::pi<float>() / BLOCK;

    constexpr int NUM_THREADS = 6;
    std::shared_ptr<std::thread> threads[NUM_THREADS];
    int vertexCount = _vertices.size();

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::make_shared<std::thread>([&, i]() {
            printf("%d\n", i);
            Random random(114514 + 28 * i);
            for (int x = i; x < vertexCount; x += NUM_THREADS) {
                auto& v = _vertices[x];
                SHEval sheval(3);
                int num = 0;
                for (int j = 0; j < BLOCK * 2; j++) {
                    for (int k = 0; k < BLOCK; k++) {
                        float phi = j * step + random.NextFloat() * step;
                        float theta = k * step + random.NextFloat() * step;
                        float r = std::sin(theta);
                        auto dir = glm::vec3(r * std::cos(phi), std::cos(theta), -r * std::sin(phi));

                        auto N = v.Normal;
                        auto P = v.Position;
                        auto cosTheta = glm::dot(N, dir);
                        if (cosTheta >= 0 && !scene->IntersectTest(Ray(P + N * 1e-5f, dir))) {
                            sheval.Project(dir, glm::vec3(cosTheta), 1.0f);
                        }
                        num++;
                    }
                }
                sheval.ScaleBy(4.f * glm::pi<float>() / num);
                v.PRT = sheval.GetSH3Mat(0);

            }
            });
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i]->join();
    }
}
