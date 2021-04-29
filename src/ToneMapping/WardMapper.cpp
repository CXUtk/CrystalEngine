#include "WardMapper.h"

WardMapper::WardMapper() {
}

WardMapper::~WardMapper() {
}

void WardMapper::Map(glm::vec3* colors, int rows, int cols) {
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

    auto S = (float)std::pow((1.219f + std::pow((Ldmax / 2), 0.4f)) / (1.219f + std::pow(Lwa, 0.4f)), 2.5f) / Ldmax;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            auto& Lw = colors[i * cols + j];
            Lw *= S;
        }
    }
}
