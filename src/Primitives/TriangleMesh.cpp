#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>> triangles) :_triangles(triangles) {
}

TriangleMesh::~TriangleMesh() {
}

BoundingBox TriangleMesh::GetBoundingBox() const {
    BoundingBox box;
    for (auto tri : _triangles) {
        box.unionWith(tri->GetBoundingBox());
    }
    return box;
}

bool TriangleMesh::Intersect(const Ray& ray, HitRecord* info) const {
    bool hit = false;
    for (auto tri : _triangles) {
        HitRecord tmp;
        if (tri->Intersect(ray, &tmp)) {
            hit = true;
            if (tmp.GetDistance() < info->GetDistance()) {
                memcpy(info, &tmp, sizeof(HitRecord));
            }
        }
    }
    return hit;
}

std::shared_ptr<TriangleMesh> TriangleMesh::CreateQuad(std::shared_ptr<Material> material, glm::mat4 transform) {
    static glm::vec3 quadVertices[4] = {
        glm::vec3(-0.5, 0, 0.5),
        glm::vec3(-0.5, 0, -0.5),
        glm::vec3(0.5, 0, -0.5),
        glm::vec3(0.5, 0, 0.5),
    };

    for (int i = 0; i < 4; i++) {
        quadVertices[i] = glm::vec3(transform * glm::vec4(quadVertices[i], 1.0f));
    }
    std::vector<std::shared_ptr<Triangle>> list;
    std::shared_ptr<Triangle> tr1 = std::shared_ptr<Triangle>(new Triangle(quadVertices[0], quadVertices[1], quadVertices[2]));
    std::shared_ptr<Triangle> tr2 = std::shared_ptr<Triangle>(new Triangle(quadVertices[0], quadVertices[2], quadVertices[3]));
    tr1->SetMaterial(material);
    tr2->SetMaterial(material);
    list.push_back(tr1);
    list.push_back(tr2);
    return std::shared_ptr<TriangleMesh>(new TriangleMesh(list));
}
