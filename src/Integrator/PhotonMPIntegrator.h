#pragma once
#include "Integrator.h"
#include <Core/Camera.h>
#include <Sampler/Sampler.h>
#include <Textures/CubemapTexture.h>
#include <Accelerator/PhotonMapping/PMKDTree.h>
class PhotonMPIntegrator : public Integrator {
public:
    PhotonMPIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler, std::shared_ptr<CubemapTexture> skybox);
    virtual ~PhotonMPIntegrator() override {}

    void Render(std::shared_ptr<const Scene> scene, std::shared_ptr<FrameBuffer> frameBuffer);
    glm::vec3 Evaluate(const Ray& ray, std::shared_ptr<const Scene> scene);

    std::shared_ptr<Camera> GetCamera() const { return _camera; }
    std::shared_ptr<Sampler> GetSampler() const { return _sampler; }
    std::shared_ptr<CubemapTexture> GetSkyBox() const { return _skyBox; }

private:
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Sampler> _sampler;
    std::shared_ptr<CubemapTexture> _skyBox;
    glm::vec3* _tmpColors;

    std::unique_ptr<PMKDTree> _photonMap;
    std::vector<Photon> _photonCollector;

    void generateGlobalMap(std::shared_ptr<const Scene> scene);
    void photonTravel(const Ray& ray, std::shared_ptr<const Scene> scene, const glm::vec3& power, int depth);
    glm::vec3 evaluate(const Ray& ray, std::shared_ptr<const Scene> scene, int depth);
};
