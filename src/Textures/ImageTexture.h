#pragma once
#include "Texture.h"
#include <string>
class ImageTexture : public Texture {
public:
    ImageTexture(const unsigned char* imgData, int width, int height, int channel);
    ImageTexture(const std::string& path);
    
    ~ImageTexture() override;

    glm::vec3 GetTexel(glm::vec2 uv) const override;

private:
    glm::vec3* _texels;
    int _width, _height, _channel;

    glm::vec3 getColor(glm::vec2 uv) const;
};