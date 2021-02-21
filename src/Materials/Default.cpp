#include "Default.h"

Default::Default(glm::vec3 color) :_color(color){
}

Default::~Default() {
}

glm::vec3 Default::BSDF(const HitRecord& hitRecord, glm::vec3 wOut, glm::vec3 wIn) {
    return _color;
}
