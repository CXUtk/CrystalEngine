#include "CubemapTexture.h"
#include "ImageTexture.h"


struct CubeUV {
    int id;
    glm::vec2 uv;
};


static glm::vec3 cubeNormals[6] = {
    glm::vec3(1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, -1),
};

static glm::vec3 cubeUVX[6] = {
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, -1),
    glm::vec3(1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(1, 0, 0),
};

static glm::vec3 cubeUVY[6] = {
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 0),
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

float CalcPreArea(const float& x, const float& y) {
    return std::atan2(x * y, std::sqrt(x * x + y * y + 1.0));
}

float CalcArea(int u_, int v_, int width,
    int height) {
    // transform from [0..res - 1] to [- (1 - 1 / res) .. (1 - 1 / res)]
    // ( 0.5 is for texel center addressing)
    float u = (2.0 * (u_ + 0.5) / width) - 1.0;
    float v = (2.0 * (v_ + 0.5) / height) - 1.0;

    // shift from a demi texel, mean 1.0 / size  with u and v in [-1..1]
    float invResolutionW = 1.0 / width;
    float invResolutionH = 1.0 / height;

    // u and v are the -1..1 texture coordinate on the current face.
    // get projected area for this texel
    float x0 = u - invResolutionW;
    float y0 = v - invResolutionH;
    float x1 = u + invResolutionW;
    float y1 = v + invResolutionH;
    float angle = CalcPreArea(x0, y0) - CalcPreArea(x0, y1) -
        CalcPreArea(x1, y0) + CalcPreArea(x1, y1);

    return angle;
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
    return _cubemaps[0]->GetTexel(uv);
}

glm::vec3 CubemapTexture::GetTexel(glm::vec3 dir) const {
    return getColor(dir);
}

void CubemapTexture::PRTProject(const std::shared_ptr<PRTEval>& evaluator) const {
    for (int i = 0; i < 6; i++) {
        int w = _cubemaps[i]->GetWidth();
        int h = _cubemaps[i]->GetHeight();
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < w; k++) {
                auto Li = _cubemaps[i]->GetTexel(glm::ivec2(k, j));
                auto uv = glm::vec2((float)j / h - 0.5f, (float)k / w - 0.5f);
                auto dir = glm::normalize(cubeNormals[i] + uv.x * cubeUVX[i] + uv.y * cubeUVY[i]);
                evaluator->Project(dir, Li, CalcArea(k, j, w, h));
            }
        }
    }
}

glm::vec3 CubemapTexture::getColor(glm::vec3 dir) const {
    auto coord = XYZ2CubeUV(dir);
    return _cubemaps[coord.id]->GetTexel(coord.uv);
}
