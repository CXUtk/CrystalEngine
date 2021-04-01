#include "ImageTexture.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#define INDEX(i, j, k) (i * _width + j) * _channel + k

ImageTexture::ImageTexture(const unsigned char* imgData, int width, int height, int channel = 3)
    : _width(width), _height(height), _channel(3) {
    _texels = new glm::vec3[_width * _height];
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            for (int k = 0; k < 3; k++) {
                _texels[i * _width + j][k] = imgData[INDEX(i, j, k)];
            }
        }
    }
}

ImageTexture::ImageTexture(const std::string& path) {
    unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &_channel, 0);
    if (!data) {
        printf("STBI loader error: %s\n", stbi_failure_reason());
        return;
    }
    _texels = new glm::vec3[_width * _height];
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            for (int k = 0; k < _channel; k++) {
                _texels[i * _width + j][k] = data[INDEX(i, j, k)] / 255.f;
            }
        }
    }
    stbi_image_free(data);
}

ImageTexture::~ImageTexture() {
    delete[] _texels;
}

glm::vec3 ImageTexture::GetTexel(glm::vec2 uv) const {
    // Binlinear clamp
    glm::vec2 mappedUV = uv * glm::vec2(_width, _height);
    glm::vec2 start = glm::floor(mappedUV);
    glm::vec2 dxdy = glm::fract(mappedUV);

    auto A = getColor(start);
    auto B = getColor(start + glm::vec2(1, 0));
    auto C = getColor(start + glm::vec2(0, 1));
    auto D = getColor(start + glm::vec2(1, 1));
    return glm::mix(glm::mix(A, B, dxdy.x), glm::mix(C, D, dxdy.x), dxdy.y);
}

glm::vec3 ImageTexture::getColor(glm::vec2 uv) const {
    int x = glm::clamp((int)(uv.x), 0, _width - 1);
    int y = glm::clamp((int)(uv.y), 0, _height - 1);
    y = _height - y - 1;
    return _texels[y * _width + x];
}
