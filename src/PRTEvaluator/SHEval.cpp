#include "SHEval.h"

SHEval::SHEval(int order) {
}

SHEval::~SHEval() {
}

void SHEval::Project(const glm::vec3& dir, const glm::vec3& Li, float dOmega) {
    for (int i = 0; i < 3; i++) {
        float c;
        c = 0.282095;
        _coeff[0][i] += Li[i] * c * dOmega;
    
        c = 0.488603;
        _coeff[1][i] += Li[i] * (c * dir.y) * dOmega;
        _coeff[2][i] += Li[i] * (c * dir.z) * dOmega;
        _coeff[3][i] += Li[i] * (c * dir.x) * dOmega;


        c = 1.092548;
        _coeff[4][i] += Li[i] * (c * dir.x * dir.y) * dOmega;
        _coeff[5][i] += Li[i] * (c * dir.y * dir.z) * dOmega;
        _coeff[7][i] += Li[i] * (c * dir.x * dir.z) * dOmega;

        c = 0.315392;
        _coeff[6][i] += Li[i] * (c * (3 * dir.z * dir.z - 1)) * dOmega;

        c = 0.546274;
        _coeff[8][i] += Li[i] * (c * (dir.x * dir.x - dir.y * dir.y)) * dOmega;
    }
}
