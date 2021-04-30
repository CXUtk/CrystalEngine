#pragma once
#include "PRTEval.h"

class SHEval : public PRTEval {
public:
    SHEval(int order);
    ~SHEval() override;

    void Project(const glm::vec3& dir, const glm::vec3& Li, float omega) override;
    void PrintCoefficient() const override;
    glm::vec3 GetShading(const SurfaceInteraction& isec) const override;
    void CalculateLight() override;
    void ScaleBy(float scale) override;
private:
    int _order;
    glm::vec3 _coeff[100];
    glm::mat4 _lightMat[3];
};
