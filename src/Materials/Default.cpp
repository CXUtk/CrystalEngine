#include "Default.h"

Default::Default(glm::vec3 color) :_color(color){
}

Default::~Default() {
}

glm::vec3 Default::Shade() {
    return _color;
}
