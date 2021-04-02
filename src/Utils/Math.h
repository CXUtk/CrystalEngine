#include <glm/glm.hpp>
inline glm::mat3 adjoint(const glm::mat3& m, float invDet) {
    glm::mat3 Inverse;
    Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
    Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet;
    Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
    Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet;
    Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet;
    Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet;
    Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet;
    Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet;
    Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;
    return Inverse;
}
