#include "CubemapTexture.h"
#include "ImageTexture.h"
#include <glm/gtx/transform.hpp>

// +x Right
// -x Left
// +y Up
// -y Down
// +z Back
// -z Front

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
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(1, 0, 0),
};

static glm::vec3 cubeUVY[6] = {
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 0, -1),
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

CubemapTexture::CubemapTexture(const std::string& posx, const std::string& negx,
    const std::string& posy, const std::string& negy,
    const std::string& posz, const std::string& negz) {
    _cubemaps[0] = std::make_shared<ImageTexture>(posx);
    _cubemaps[1] = std::make_shared<ImageTexture>(negx);
    _cubemaps[2] = std::make_shared<ImageTexture>(posy);
    _cubemaps[3] = std::make_shared<ImageTexture>(negy);
    _cubemaps[4] = std::make_shared<ImageTexture>(posz);
    _cubemaps[5] = std::make_shared<ImageTexture>(negz);
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
    float totWeight = 0;
    for (int i = 0; i < 6; i++) {
        int w = _cubemaps[i]->GetWidth();
        int h = _cubemaps[i]->GetHeight();
        auto c = _cubemaps[i]->GetTexel(glm::ivec2(0, 0));
        printf("%lf %lf %lf\n", c.r, c.g, c.b);
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < w; k++) {
                auto Li = _cubemaps[i]->GetTexel(glm::ivec2(k, j));
                auto uv = glm::vec2((float)(k + 0.5) / w - 0.5f, (float)(j + 0.5) / h - 0.5f);
                auto dir = glm::normalize(cubeNormals[i] * 0.5f + uv.x * cubeUVX[i] + uv.y * cubeUVY[i]);
                auto dw = CalcArea(k, j, w, h);
                evaluator->Project(dir, Li, dw);
                totWeight += dw;
            }
        }
    }
    evaluator->ScaleBy(4 * glm::pi<float>() / totWeight);
    printf("%lf\n", totWeight);
}

glm::vec3 CubemapTexture::getColor(glm::vec3 dir) const {
    auto coord = XYZ2CubeUV(dir);
    return _cubemaps[coord.id]->GetTexel(coord.uv);
}
