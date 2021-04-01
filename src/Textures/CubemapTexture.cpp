#include "CubemapTexture.h"

struct CubeUV {
    int id;
    glm::vec2 uv;
};


inline CubeUV XYZ2CubeUV(const glm::vec3& p) {
    float ax = std::abs(p.x);
    float ay = std::abs(p.y);
    float az = std::abs(p.z);
    CubeUV c;
    if (ax >= ay && ax >= az)   // x face
    {
        c = { p.x >= 0 ? 0 : 1, glm::vec2(-p.z / p.x, p.y / ax) };
    }
    else if (ay >= az)  // y face
    {
        c = { p.y >= 0 ? 2 : 3, glm::vec2(p.x / ay, -p.z / p.y) };
    }
    else // z face
    {
        c = { p.z >= 0 ? 4 : 5, glm::vec2(p.x / p.z, p.y / az) };
    }
    c.uv.x = c.uv.x * 0.5f + 0.5f;
    c.uv.y = c.uv.y * 0.5f + 0.5f;
    return c;
}

CubemapTexture::CubemapTexture(const std::string& right,
    const std::string& left,
    const std::string& top,
    const std::string& bottom,
    const std::string& front,
    const std::string& back) {
    _cubemaps[0] = std::make_shared<ImageTexture>(right);
    _cubemaps[1] = std::make_shared<ImageTexture>(left);
    _cubemaps[2] = std::make_shared<ImageTexture>(top);
    _cubemaps[3] = std::make_shared<ImageTexture>(bottom);
    _cubemaps[4] = std::make_shared<ImageTexture>(front);
    _cubemaps[5] = std::make_shared<ImageTexture>(back);
}

CubemapTexture::~CubemapTexture() {
}

glm::vec3 CubemapTexture::GetTexel(glm::vec2 uv) const {
    return glm::vec3();
}

glm::vec3 CubemapTexture::GetTexel(glm::vec3 dir) const {
    return getColor(dir);
}

glm::vec3 CubemapTexture::getColor(glm::vec3 dir) const {
    auto coord = XYZ2CubeUV(dir);
    return _cubemaps[coord.id]->GetTexel(coord.uv);
}
