﻿#include "Lighted.h"

Lighted::Lighted(glm::vec3 color) :_color(color) {
}

Lighted::~Lighted() {
}

glm::vec3 Lighted::BSDF(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut, glm::vec3 wIn, bool& shouldBounce) {
    shouldBounce = false;
    return glm::vec3(0);
}

glm::vec3 Lighted::Le(const SurfaceInteraction& SurfaceInteraction, glm::vec3 wOut) {
    return _color;
}
