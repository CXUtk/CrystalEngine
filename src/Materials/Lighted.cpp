#include "Lighted.h"

Lighted::Lighted(glm::vec3 color) :_color(color) {
}

Lighted::~Lighted() {
}

glm::vec3 Lighted::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) {
    return glm::vec3(0);
}

glm::vec3 Lighted::Le(const HitRecord& hitRecord, glm::vec3 wOut) {
    return _color;
}
