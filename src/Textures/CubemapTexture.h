#pragma once
#include "Texture.h"
#include <string>
#include <memory>
class ImageTexture;
class CubemapTexture : public Texture {
public:
    CubemapTexture(const std::string& right, const std::string& left,
        const std::string& top, const std::string& bottom,
        const std::string& front, const std::string& back);

    ~CubemapTexture() override;

    glm::vec3 GetTexel(glm::vec2 uv) const override;
    glm::vec3 GetTexel(glm::vec3 dir) const;

private:
    std::shared_ptr<ImageTexture> _cubemaps[6];

    glm::vec3 getColor(glm::vec3 dir) const;
};
