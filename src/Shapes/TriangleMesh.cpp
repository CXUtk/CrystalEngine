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
    const glm::mat4 transform,
    const std::shared_ptr<Light>& light) : _vertices(Vertices), _material(material), _transform(transform), _light(light) {
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


void computeTangentVectors(const glm::vec3& N, glm::vec3& T, glm::vec3& B) {
    for (int j = 0; j < 3; j++) {
        glm::vec3 v(0);
        v[j] = 1;
        if (glm::cross(N, v) != glm::vec3(0)) {
            T = glm::normalize(glm::cross(N, v));
            B = glm::normalize(glm::cross(N, T));
            break;
        }
    }
}

void TriangleMesh::ComputeInitialRadianceTransfer(const std::shared_ptr<Scene>& scene) {
    constexpr int NUM_SAMPLES = 1000;
    int BLOCK = std::sqrt(NUM_SAMPLES);
    float step = 1.f / BLOCK;

    constexpr int NUM_THREADS = 6;
    std::shared_ptr<std::thread> threads[NUM_THREADS];
    int vertexCount = _vertices.size();
    std::mutex mutexLock;
    int cnt = 0;

    // Criterion 1: The information stored by the spherical harmonic function is irradiance
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::make_shared<std::thread>([&, i]() {
            printf("Thread: %d\n", i);

            for (int x = i; x < vertexCount; x += NUM_THREADS) {
                auto& v = _vertices[x];
                SHEval sheval(3);
                int num = 0;
                auto N = v.Normal;
                auto P = v.Position;
                glm::vec3 T, B;
                computeTangentVectors(N, T, B);
                Random random(114514 + glm::dot(N, glm::vec3(0.875641, 0.6182447, -0.5897723)) * 1048576);
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

                v.PRT[0] = sheval.GetSH3Mat(0);
                v.PRT[1] = sheval.GetSH3Mat(1);
                v.PRT[2] = sheval.GetSH3Mat(2);

                mutexLock.lock();
                cnt++;
                printf("Default: %.2lf%%\n", (float)cnt / vertexCount * 100.0);
                mutexLock.unlock();
            }
            });
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i]->join();
    }


}

void TriangleMesh::ComputeInterReflection(const std::shared_ptr<Scene>& scene) {
    constexpr int NUM_SAMPLES = 1000;
    int BLOCK = std::sqrt(NUM_SAMPLES);
    float step = 1.f / BLOCK;

    constexpr int NUM_THREADS = 6;
    std::shared_ptr<std::thread> threads[NUM_THREADS];
    int vertexCount = _vertices.size();
    std::mutex mutexLock;
    int cnt = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::make_shared<std::thread>([&, i]() {
            for (int x = i; x < vertexCount; x += NUM_THREADS) {
                auto& v = _vertices[x];
                SHEval sheval(3);
                int num = 0;
                auto N = v.Normal;
                auto P = v.Position;
                glm::vec3 T, B;
                computeTangentVectors(N, T, B);
                Random random(114514 + glm::dot(N, glm::vec3(0.875641, 0.6182447, -0.5897723)) * 1048576);
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
                        SurfaceInteraction isec;
                        if (scene->Intersect(Ray(P + N * 1e-5f, dir), &isec)) {
                            auto material = isec.GetHitObject()->GetMaterial();
                            auto bsdf = material->ComputeScatteringFunctions(isec);
                            auto color = bsdf->DistributionFunction(-dir, isec.GetNormal());

                            sheval.Append(color.r * isec.GetRadianceTransfer(0), 0);
                            sheval.Append(color.g * isec.GetRadianceTransfer(1), 1);
                            sheval.Append(color.b * isec.GetRadianceTransfer(2), 2);
                        }
                        num++;
                    }
                }
                sheval.ScaleBy(glm::pi<float>() / num);
                v.PRT_t[0] = v.PRT[0] + sheval.GetSH3Mat(0);
                v.PRT_t[1] = v.PRT[1] + sheval.GetSH3Mat(1);
                v.PRT_t[2] = v.PRT[2] + sheval.GetSH3Mat(2);

                mutexLock.lock();
                cnt++;
                printf("Inter-reflection: %.2lf%%\n", (float)cnt / vertexCount * 100.0);
                mutexLock.unlock();
            }
            });
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i]->join();
    }
    for (int x = 0; x < vertexCount; x++) {
        auto& v = _vertices[x];
        for (int j = 0; j < 3; j++) {
            std::swap(v.PRT[j], v.PRT_t[j]);
        }
    }
}

void TriangleMesh::WritePRTInfo(FILE* file) {
    fprintf(file, "%lld\n", _vertices.size());
    fprintf(file, "EDIT_COLOR_R EDIT_COLOR_G EDIT_COLOR_B\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fprintf(file, "%lf ", _transform[i][j]);
        }
        fprintf(file, "\n");
    }
    // First 3V lines are PRT valus without inter-reflection
    for (auto& v : _vertices) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                fprintf(file, "%lf %lf %lf ", v.PRT_t[i][j].x, v.PRT_t[i][j].y, v.PRT_t[i][j].z);
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }
    // Second 3V lines are PRT values with inter-reflection
    for (auto& v : _vertices) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                fprintf(file, "%lf %lf %lf ", v.PRT[i][j].x, v.PRT[i][j].y, v.PRT[i][j].z);
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }
}
