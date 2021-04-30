#pragma once
#include <glm/glm.hpp>
class PRTEval {
public:
    PRTEval() {}
    virtual ~PRTEval() = 0 {}

    virtual void Project(const glm::vec3& dir, const glm::vec3& Li, float omega) = 0;
};