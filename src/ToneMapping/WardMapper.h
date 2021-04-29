#pragma once
#include "ToneMapper.h"

class WardMapper : public ToneMapper {
public:
    WardMapper();
    ~WardMapper() override;

    void Map(glm::vec3* colors, int rows, int cols) override;
private:
};