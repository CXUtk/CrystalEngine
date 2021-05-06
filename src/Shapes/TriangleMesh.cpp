#include "TriangleMesh.h"
#include "Core/GeometryObject.h"
#include <Core/Scene.h>
#include <glm/gtx/transform.hpp>
#include <Utils/Random.h>
#include <PRTEvaluator/SHEval.h>
#include <thread>
#include <cstdio>
#include <mutex>


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
    for (auto& v : _vertices) {
        box = box.Union(v.Position);
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
    constexpr int NUM_SAMPLES = 1000;
    int BLOCK = std::sqrt(NUM_SAMPLES);
    float step = 1.f / BLOCK;

    constexpr int NUM_THREADS = 6;
    std::shared_ptr<std::thread> threads[NUM_THREADS];
    int vertexCount = _vertices.size();
    std::mutex mutexLock;
    int cnt = 0;
    printf("%d\n", vertexCount);
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::make_shared<std::thread>([&, i]() {
            printf("%d\n", i);

            for (int x = i; x < vertexCount; x += NUM_THREADS) {
                auto& v = _vertices[x];
                SHEval sheval(3);
                int num = 0;
                auto N = v.Normal;
                auto P = v.Position;
                glm::vec3 T, B;
                for (int j = 0; j < 3; j++) {
                    glm::vec3 v(0);
                    v[j] = 1;
                    if (glm::cross(N, v) != glm::vec3(0)) {
                        T = glm::normalize(glm::cross(N, v));
                        B = glm::normalize(glm::cross(N, T));
                        break;
                    }
                }
                Random random(114514 + glm::dot(N, glm::vec3(0.2132, 0.6182, -0.7112)) * 1048576);
                for (int j = 0; j < BLOCK; j++) {
                    float phi = j * step + random.NextFloat() * step;
                    phi *= glm::two_pi<float>();
                    float cosphi = std::sin(phi);
                    float sinphi = std::cos(phi);
                    for (int k = 0; k < BLOCK; k++) {
                        float r = k * step + random.NextFloat() * step;
                        r = std::sqrt(r);
                        auto dir = glm::vec3(r * sinphi, std::sqrt(1 - r * r), r * cosphi);
                        auto cosTheta = dir.y;
                        dir = dir.x * T + dir.y * N + dir.z * B;
                        //sheval.Project(dir, glm::vec3(cosTheta), 1.0f);
                        if (!scene->IntersectTest(Ray(P + N * 1e-5f, dir))) {
                            sheval.Project(dir, glm::vec3(1), 1.0f);
                        }
                        num++;
                    }
                }
                sheval.ScaleBy(glm::pi<float>() / num);
                v.PRT = sheval.GetSH3Mat(0);
                mutexLock.lock();
                cnt++;
                printf("%.2lf%%\n", (float)cnt / vertexCount * 100.0);
                mutexLock.unlock();
            }
            });
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i]->join();
    }


    FILE* output = fopen("PRT.txt", "w");
    fprintf(output, "%lld\n", _vertices.size());
    for (auto& v : _vertices) {
        for (int i = 0; i < 3; i++) {
            fprintf(output, "%lf %lf %lf ", v.PRT[i].x, v.PRT[i].y, v.PRT[i].z);
        }
        fprintf(output, "\n");
    }
    fclose(output);
}
