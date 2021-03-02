#include "Strauss.h"
#include <Utils/Math.h>


#define SQR(x) ((x) * (x))


float fresnel(float x) {
    float kf = 1.12;
    float t = 1 / (kf * kf);
    float sqr = 1 / SQR(x - kf);
    float above = sqr - t;
    float below = 1 / SQR(1 - kf) - t;
    return above / below;
}
float geometric(float x) {
    float kg = 1.01;
    float sqr = 1 / SQR(1 - kg);
    float above = sqr - 1 / SQR(x - kg);
    float below = sqr - 1 / (kg * kg);
    return above / below;
}


Strauss::Strauss(glm::vec3 baseColor, float smoothness, float metalness, float transparency) : _baseColor(baseColor),
_smoothness(smoothness), _metalness(metalness), _transparency(transparency) {

}

Strauss::~Strauss() {
}

glm::vec3 Strauss::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) {
    glm::vec3 V = -wOut;
    glm::vec3 L = -wIn;
    glm::vec3 N = hitRecord.GetNormal();
    glm::vec3 H = glm::normalize(glm::reflect(L, N));

    auto alpha = glm::dot(N, L);
    auto beta = glm::dot(H, V);
    auto gamma = glm::dot(N, V);

    // Diffuse
    float rd = (1 - _smoothness * _smoothness * _smoothness) * (1 - _transparency);
    float d = 1 - _smoothness * _metalness;
    glm::vec3 Qd = glm::max(dot(N, -L) * d * rd * _baseColor, glm::vec3(0));


    // Specular factor Qs
    float h = 3 / (1 - _smoothness);
    float rn = (1 - _transparency) - rd;
    float kj = 0.1;
    float j = fresnel(alpha) * geometric(alpha) * geometric(gamma);
    float rj = glm::min(1.0f, rn + (rn + kj) * j);

    float s = -glm::dot(H, V);
    if (s < 0) return Qd;
    float rs = glm::pow(s, h) * rj;
    glm::vec3 white = glm::vec3(1, 1, 1);
    glm::vec3 Cs = white + _metalness * (1 - fresnel(alpha)) * (_baseColor - white);
    glm::vec3 Qs = glm::max(rs * Cs, glm::vec3(0));
    return Qd + Qs;
}

glm::vec3 Strauss::Le(const HitRecord& hitRecord, glm::vec3 wOut) {
    return _baseColor * 0.1f;
}
