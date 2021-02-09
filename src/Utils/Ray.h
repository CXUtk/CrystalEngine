#pragma once
#include <glm/glm.hpp>
struct Ray {
    glm::vec3 start, dir;
    Ray() { start = glm::vec3(0), dir = glm::vec3(0); }
    Ray(glm::vec3 start, glm::vec3 dir) :start(start), dir(dir) {}
};
