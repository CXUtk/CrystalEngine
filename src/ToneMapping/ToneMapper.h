#pragma once
#include <glm/glm.hpp>

constexpr glm::vec3 LUMINANCE_VEC = glm::vec3(0.299, 0.587, 0.114);

class ToneMapper {
public:
    ToneMapper() {}
    virtual ~ToneMapper() = 0 {}

    virtual void Map(glm::vec3* colors, int rows, int cols) = 0;
};
