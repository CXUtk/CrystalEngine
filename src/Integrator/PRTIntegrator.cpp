#include "PRTIntegrator.h"
#include <PRTEvaluator/SHEval.h>
static constexpr float EPS = 1e-5;

PRTIntegrator::PRTIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, std::shared_ptr<CubemapTexture> skybox)
    : SamplerIntegrator(camera, sampler, skybox) {
    printf("Started projecting SH function...\n");
    _prt = std::make_shared<SHEval>(3);
    GetSkyBox()->PRTProject(_prt);
    printf("Environment map ready!\n");
}

PRTIntegrator::~PRTIntegrator() {
}

void PRTIntegrator::Preprocess(const std::shared_ptr<Scene>& scene) {
    _prt->CalculateLight();
    _prt->PrintCoefficient();
    for (auto& triangles : scene->GetTriangleMeshes()) {
        triangles->ComputeInitialRadianceTransfer(scene);
    }
    for (auto& triangles : scene->GetTriangleMeshes()) {
        triangles->ComputeInterReflection(scene);
    }
    FILE* file = fopen("PRT.scene", "w");
    int size = scene->GetTriangleMeshes().size();
    fprintf(file, "%d\n", size);
    for (auto& triangles : scene->GetTriangleMeshes()) {
        fprintf(file, "Edit_Here\n");
        triangles->WritePRTInfo(file);
    }
    fclose(file);
}

glm::vec3 PRTIntegrator::Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene) {
    glm::vec3 L(0);
    SurfaceInteraction hit;
    if (scene->Intersect(ray, &hit)) {
        glm::vec3 hitPos = hit.GetHitPos();
        glm::vec3 N = hit.GetNormal();
        glm::vec3 wOut = -ray.dir;
        auto objHit = hit.GetHitObject();

        auto material = objHit->GetMaterial();
        if (material == nullptr) return L;

        auto bsdf = objHit->ComputeScatteringFunctions(hit);
        if (bsdf == nullptr) {
            return Evaluate(hit.SpawnRay(ray.dir), scene);
        }

        for (auto& light : scene->GetLights()) {
            glm::vec3 lightP;
            float pdf;
            auto radiance = light->SampleLi(hit, lightP, &pdf);
            if (pdf == 0.0) continue;

            auto v = lightP - hitPos;
            auto dir = glm::normalize(v);
            SurfaceInteraction hit2;

            float distance = glm::length(v);
            if (!scene->IntersectTest(Ray(hitPos + N * EPS, dir), 0, distance - EPS)) {
                L += radiance * bsdf->DistributionFunction(wOut, dir) * std::max(0.f, glm::dot(N, dir)) / pdf;
            }
        }
        L += _prt->GetShading(hit, ray.dir);
        return L;
    }
    if (GetSkyBox() == nullptr) return L;
    return GetSkyBox()->GetTexel(ray.dir);
}
