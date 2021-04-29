#pragma once
#include "ToneMapper.h"

class ReinhardMapper : public ToneMapper {
public:
    ReinhardMapper();
    ~ReinhardMapper() override;

    void Map(glm::vec3* colors, int rows, int cols) override;
private:
};