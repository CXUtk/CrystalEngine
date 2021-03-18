#include "ImageTexture.h"
#include <vector>
#include <lodepng/lodepng.h>

#define INDEX(i, j, k) (i * _width + j) * 4 + k

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
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error;

    if (error = lodepng::load_file(png, path)) {
        printf("PNG decoder error %d: %s\n", error, lodepng_error_text(error));
        return;
    }
    error = lodepng::decode(image, width, height, png);
    if (error) {
        printf("PNG decoder error %d: %s\n", error, lodepng_error_text(error));
        return;
    }
    _width = width, _height = height, _channel = 4;
    _texels = new glm::vec3[_width * _height];
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            for (int k = 0; k < 3; k++) {
                _texels[i * _width + j][k] = image.data()[INDEX(i, j, k)] / 255.f;
            }
        }
    }
}

ImageTexture::~ImageTexture() {
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
    return _texels[y * _width + x];
}