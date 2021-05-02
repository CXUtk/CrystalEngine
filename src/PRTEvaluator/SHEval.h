#pragma once
#include "PRTEval.h"
#include <vector>

class SHEval : public PRTEval {
public:
    SHEval(int order);
    ~SHEval() override;

    void Project(const glm::vec3& dir, const glm::vec3& Li, float omega) override;
    void PrintCoefficient() const override;
    glm::vec3 GetShading(const SurfaceInteraction& isec) const override;
    void CalculateLight() override;
    void ScaleBy(float scale) override;
    glm::mat3 GetSH3Mat(int index) const;
    std::vector<glm::mat3> GetSH3RGBMat() const;
private:
    int _order;
    glm::vec3 _coeff[10];
    glm::mat4 _lightMat[3];
};
