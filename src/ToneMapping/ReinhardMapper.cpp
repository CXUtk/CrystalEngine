#include "ReinhardMapper.h"

ReinhardMapper::ReinhardMapper() {
}

ReinhardMapper::~ReinhardMapper() {
}

void ReinhardMapper::Map(glm::vec3* colors, int rows, int cols) {
    auto Ldmax = 1.0f;
    auto Lwa = 0.f;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            auto L = glm::dot(LUMINANCE_VEC, colors[i * cols + j]);
            Lwa += log(L + 0.05);
        }
    }
    Lwa /= (float)rows * cols;
    Lwa = exp(Lwa);

    auto scale = 0.18f / Lwa;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            auto& color = colors[i * cols + j];
            auto scaled = scale * color;
            color.r = color.r / (1.0f + color.r);
            color.g = color.g / (1.0f + color.g);
            color.b = color.b / (1.0f + color.b);
        }
    }
}
