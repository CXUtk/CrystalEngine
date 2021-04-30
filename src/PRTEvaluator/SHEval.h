#pragma once
#include "PRTEval.h"
class SHEval : public PRTEval {
public:
    SHEval(int order);
    ~SHEval() override;

    void Project(const glm::vec3& dir, const glm::vec3& Li, float omega) override;

private:
    glm::vec3 _coeff[9];
};