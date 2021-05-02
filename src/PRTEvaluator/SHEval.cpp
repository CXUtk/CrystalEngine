#include "SHEval.h"
#include <Core/SurfaceInteraction.h>
SHEval::SHEval(int order) :_order(order) {
    memset(_coeff, 0, sizeof(_coeff));
}

SHEval::~SHEval() {
}

void SHEval3(const float fX, const float fY, const float fZ, float* pSH) {
    float fC0, fC1, fS0, fS1, fTmpA, fTmpB, fTmpC;
    float fZ2 = fZ * fZ;

    pSH[0] = 0.2820947917738781f;
    pSH[2] = 0.4886025119029199f * fZ;
    pSH[6] = 0.9461746957575601f * fZ2 + -0.3153915652525201f;
    fC0 = fX;
    fS0 = fY;

    fTmpA = -0.48860251190292f;
    pSH[3] = fTmpA * fC0;
    pSH[1] = fTmpA * fS0;
    fTmpB = -1.092548430592079f * fZ;
    pSH[7] = fTmpB * fC0;
    pSH[5] = fTmpB * fS0;
    fC1 = fX * fC0 - fY * fS0;
    fS1 = fX * fS0 + fY * fC0;

    fTmpC = 0.5462742152960395f;
    pSH[8] = fTmpC * fC1;
    pSH[4] = fTmpC * fS1;
}

void SHEval::Project(const glm::vec3& dir, const glm::vec3& Li, float dOmega) {
    //float SH[9];
    //SHEval3(dir.x, dir.y, dir.z, SH);
    for (int i = 0; i < 3; i++) {
        //for (int j = 0; j < 9; j++) {
        //    _coeff[j][i] += Li[i] * SH[j] * dOmega;
        //}
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

void SHEval::PrintCoefficient() const {
    for (int i = 0; i < _order * _order; i++) {
        printf("%.6lf %.6lf %.6lf\n", _coeff[i].r, _coeff[i].g, _coeff[i].b);
    }
}

float myDot(glm::mat3 A, glm::mat3 B) {
    return glm::dot(A[0], B[0]) + glm::dot(A[1], B[1]) + glm::dot(A[2], B[2]);
}

glm::vec3 SHEval::GetShading(const SurfaceInteraction& isec) const {
    //auto normal = isec.GetNormal();
    //glm::vec4 N = glm::vec4(normal, 1.0f);
    //return glm::vec3(glm::dot(N, _lightMat[0] * N), glm::dot(N, _lightMat[1] * N), glm::dot(N, _lightMat[2] * N));
    auto prt = isec.GetRadianceTransfer();
    return glm::vec3(myDot(prt, GetSH3Mat(0)), myDot(prt, GetSH3Mat(1)), myDot(prt, GetSH3Mat(2)));
}

void SHEval::CalculateLight() {

    /* Form the quadratic form matrix (see equations 11 and 12 in paper) */

    int col;
    float c1, c2, c3, c4, c5;
    c1 = 0.429043; c2 = 0.511664;
    c3 = 0.743125; c4 = 0.886227; c5 = 0.247708;

    for (col = 0; col < 3; col++) { /* Equation 12 */
        _lightMat[col][0][0] = c1 * _coeff[8][col]; /* c1 L_{22}  */
        _lightMat[col][0][1] = c1 * _coeff[4][col]; /* c1 L_{2-2} */
        _lightMat[col][0][2] = c1 * _coeff[7][col]; /* c1 L_{21}  */
        _lightMat[col][0][3] = c2 * _coeff[3][col]; /* c2 L_{11}  */

        _lightMat[col][1][0] = c1 * _coeff[4][col]; /* c1 L_{2-2} */
        _lightMat[col][1][1] = -c1 * _coeff[8][col]; /*-c1 L_{22}  */
        _lightMat[col][1][2] = c1 * _coeff[5][col]; /* c1 L_{2-1} */
        _lightMat[col][1][3] = c2 * _coeff[1][col]; /* c2 L_{1-1} */

        _lightMat[col][2][0] = c1 * _coeff[7][col]; /* c1 L_{21}  */
        _lightMat[col][2][1] = c1 * _coeff[5][col]; /* c1 L_{2-1} */
        _lightMat[col][2][2] = c3 * _coeff[6][col]; /* c3 L_{20}  */
        _lightMat[col][2][3] = c2 * _coeff[2][col]; /* c2 L_{10}  */

        _lightMat[col][3][0] = c2 * _coeff[3][col]; /* c2 L_{11}  */
        _lightMat[col][3][1] = c2 * _coeff[1][col]; /* c2 L_{1-1} */
        _lightMat[col][3][2] = c2 * _coeff[2][col]; /* c2 L_{10}  */
        _lightMat[col][3][3] = c4 * _coeff[0][col] - c5 * _coeff[6][col];
        /* c4 L_{00} - c5 L_{20} */
    }
}

void SHEval::ScaleBy(float scale) {
    for (int i = 0; i < _order * _order; i++) {
        _coeff[i] *= scale;
    }
}

glm::mat3 SHEval::GetSH3Mat(int index) const {
    return glm::mat3(glm::vec3(_coeff[0][index], _coeff[1][index], _coeff[2][index]),
        glm::vec3(_coeff[3][index], _coeff[4][index], _coeff[5][index]),
        glm::vec3(_coeff[6][index], _coeff[7][index], _coeff[8][index]));
}

std::vector<glm::mat3> SHEval::GetSH3RGBMat() const {
    std::vector<glm::mat3> res;
    res.push_back(GetSH3Mat(0));
    res.push_back(GetSH3Mat(1));
    res.push_back(GetSH3Mat(2));
    return res;
}
