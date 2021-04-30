#pragma once
#include "Texture.h"
#include <PRTEvaluator/PRTEval.h>
#include <string>
#include <memory>
class ImageTexture;


class CubemapTexture : public Texture {
public:
    CubemapTexture(const std::string& posx, const std::string& negx,
        const std::string& posy, const std::string& negy,
        const std::string& posz, const std::string& negz);

    ~CubemapTexture() override;

    glm::vec3 GetTexel(glm::vec2 uv) const override;
    glm::vec3 GetTexel(glm::vec3 dir) const;

    void PRTProject(const std::shared_ptr<PRTEval>& evaluator) const;

private:
    std::shared_ptr<ImageTexture> _cubemaps[6];

    glm::vec3 getColor(glm::vec3 dir) const;
};
