#include "Triangle.h"
#include "Utils/Math.h"
#include <glm/gtx/transform.hpp>
#include <Core/Scene.h>
#include <Utils/Random.h>
#include <PRTEvaluator/SHEval.h>

constexpr bool FLAT_SHADING = false;
constexpr float EPS = 1e-7;

static Random random;

glm::vec3 bary_interp(glm::vec3 bary, glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    return bary.x * A + bary.y * B + bary.z * C;
}
glm::vec2 bary_interp(glm::vec3 bary, glm::vec2 A, glm::vec2 B, glm::vec2 C) {
    return bary.x * A + bary.y * B + bary.z * C;
}

Triangle::Triangle(const VertexData& a, const VertexData& b, const VertexData& c) {
    _vertices[0] = a, _vertices[1] = b, _vertices[2] = c;
    calculateDerivative();
}

Triangle::~Triangle() {
}

BoundingBox Triangle::GetBoundingBox() const {
    glm::vec3 minn = _vertices[0].Position, maxx = _vertices[0].Position;
    for (int i = 1; i < 3; i++) {
        minn.x = std::min(minn.x, _vertices[i].Position.x);
        minn.y = std::min(minn.y, _vertices[i].Position.y);
        minn.z = std::min(minn.z, _vertices[i].Position.z);

        maxx.x = std::max(maxx.x, _vertices[i].Position.x);
        maxx.y = std::max(maxx.y, _vertices[i].Position.y);
        maxx.z = std::max(maxx.z, _vertices[i].Position.z);

    }
    return BoundingBox(minn, maxx);
}

bool Triangle::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    glm::mat3 A(_vertices[1].Position - _vertices[0].Position, _vertices[2].Position - _vertices[0].Position, -ray.dir);
    auto det = glm::determinant(A);
    if (std::abs(det) < EPS) return false;
    auto inv = adjoint(A, 1 / det);
    glm::vec3 P = ray.start - _vertices[0].Position;
    auto res = inv * P;
    if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1.0 + EPS || res.z < 0) return false;
    glm::vec3 bary_coord = glm::vec3(1 - res.x - res.y, res.x, res.y);
    glm::vec3 N;
    glm::vec2 UV = bary_interp(bary_coord, _vertices[0].TexCoords, _vertices[1].TexCoords, _vertices[2].TexCoords);
    if (FLAT_SHADING) {
        N = glm::normalize(glm::cross(_vertices[1].Position - _vertices[0].Position, _vertices[2].Position - _vertices[0].Position));
    }
    else {
        N = glm::normalize(bary_interp(bary_coord, _vertices[0].Normal, _vertices[1].Normal, _vertices[2].Normal));
    }
    auto front_face = glm::dot(ray.dir, N) < 0;
    N = front_face ? N : -N;


    auto dpdu = _dpdu;
    // If there are no tangent vector attached
    if (dpdu == glm::vec3(0)) {
        for (int i = 0; i < 3; i++) {
            glm::vec3 v(0);
            v[i] = 1;
            auto tmp = glm::cross(v, N);
            if (tmp != glm::vec3(0)) {
                dpdu = glm::normalize(tmp);
                break;
            }
        }
    }

    glm::mat3 prt = _vertices[0].PRT * bary_coord[0] + _vertices[1].PRT * bary_coord[1] + _vertices[2].PRT * bary_coord[2];
    info->SetRadianceTransfer(prt);
    info->SetHitInfo(res.z, ray.start + ray.dir * res.z, N, UV, front_face, this, dpdu, glm::cross(N, dpdu));
    return true;
}

bool Triangle::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    if (tMin > tMax) return false;
    glm::mat3 A(_vertices[1].Position - _vertices[0].Position, _vertices[2].Position - _vertices[0].Position, -ray.dir);
    auto det = glm::determinant(A);
    if (std::abs(det) < EPS) return false;
    auto inv = adjoint(A, 1 / det);
    glm::vec3 P = ray.start - _vertices[0].Position;
    auto res = inv * P;
    if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;
    return res.z >= tMin && res.z <= tMax;
}

void Triangle::ApplyTransform(glm::mat4 transform, glm::mat4 normalTransfrom) {
    for (int i = 0; i < 3; i++) {
        _vertices[i].Position = glm::vec3(transform * glm::vec4(_vertices[i].Position, 1.0f));
        _vertices[i].Normal = glm::vec3(normalTransfrom * glm::vec4(_vertices[i].Normal, 0.0f));
    }
    calculateDerivative();
}

void Triangle::ComputeTransferFunction(const std::shared_ptr<Scene>& scene) {

    constexpr int NUM_SAMPLES = 200;
    int BLOCK = std::sqrt(NUM_SAMPLES / 2);
    float step = glm::pi<float>() / BLOCK;

    for (int i = 0; i < 3; i++) {
        memset(&_vertices[i].PRT, 0, sizeof(glm::mat3));
    }
    SHEval sheval(3);
    int num = 0;
    for (int j = 0; j < BLOCK * 2; j++) {
        for (int k = 0; k < BLOCK; k++) {
            glm::vec3 vec(0);
            float phi = j * step + random.NextFloat() * step;
            float theta = k * step + random.NextFloat() * step;
            float r = std::sin(theta);
            auto dir = glm::vec3(r * std::cos(phi), std::cos(theta), -r * std::sin(phi));

            for (int i = 0; i < 3; i++) {
                auto N = _vertices[i].Normal;
                auto P = _vertices[i].Position;
                auto v = glm::dot(N, dir);
                if (v >= 0 && !scene->IntersectTest(Ray(P + N * 1e-5f, dir))) {
                    vec[i] = v;
                }
            }
            sheval.Project(dir, vec, 1.0f);
            num++;
        }
    }
    sheval.ScaleBy(4.f * glm::pi<float>() / num);
    for (int i = 0; i < 3; i++) {
        _vertices[i].PRT = sheval.GetSH3Mat(i);
    }
    printf("Finish\n");
}



void Triangle::calculateDerivative() {
    glm::mat2 A(
        glm::vec2(_vertices[0].TexCoords.x - _vertices[2].TexCoords.x, _vertices[0].TexCoords.y - _vertices[2].TexCoords.y),
        glm::vec2(_vertices[1].TexCoords.x - _vertices[2].TexCoords.x, _vertices[1].TexCoords.y - _vertices[2].TexCoords.y)
    );
    A = glm::inverse(A);
    if (glm::isnan(A[0]) != glm::bvec2(false) || glm::isnan(A[1]) != glm::bvec2(false)) {
        _dpdu = _dpdv = glm::vec3(0);
    }
    else {
        auto a = _vertices[0].Position - _vertices[2].Position;
        auto b = _vertices[1].Position - _vertices[2].Position;
        auto res = A * glm::mat3x2(glm::vec2(a.x, b.x), glm::vec2(a.y, b.y), glm::vec2(a.z, b.z));
        _dpdu = glm::vec3(res[0][0], res[1][0], res[2][0]);
        _dpdv = glm::vec3(res[0][1], res[1][1], res[2][1]);
    }
}

//void Triangle::ApplyTransform(glm::mat4 transform) {
//    for (int i = 0; i < 3; i++) {
//        _vertices[i] = transform * glm::vec4(_vertices[i], 1);
//    }
//}
//
//glm::vec3 Triangle::GetRealHitPosAndNormal(glm::vec3 hitpos, glm::vec3 rayDir) const {
//    return 
//}
