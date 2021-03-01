#include "TriangleMesh.h"
#include <Primitives/Primitives.h>

TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>> triangles, glm::mat4 matrix) : _triangles(triangles) {
    transform(matrix);
    _acceleator = Accelerator::GetAccelerator("KDTree");
    std::vector<std::shared_ptr<Object>> objects(_triangles.begin(), _triangles.end());
    _acceleator->Build(objects);
}

TriangleMesh::~TriangleMesh() {
}

BoundingBox TriangleMesh::GetBoundingBox() const {
    BoundingBox box;
    for (auto& tri : _triangles) {
        box = box.Union(tri->GetBoundingBox());
    }
    return box;
}

bool TriangleMesh::Intersect(const Ray& ray, HitRecord* info) const {
    return _acceleator->Intersect(ray, info);
    //bool hit = false;
    //for (auto& tri : _triangles) {
    //    HitRecord tmp;
    //    if (tri->Intersect(ray, &tmp)) {
    //        hit = true;
    //        if (tmp.GetDistance() < info->GetDistance()) {
    //            memcpy(info, &tmp, sizeof(HitRecord));
    //        }
    //    }
    //}
    //return hit;
}

std::shared_ptr<TriangleMesh> TriangleMesh::CreateQuad(std::shared_ptr<Material> material, glm::mat4 transform) {
    static VertexData quadVertices[4] = {
        { glm::vec3(-0.5, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0)  },
        { glm::vec3(-0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
        { glm::vec3(0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
        { glm::vec3(0.5, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0)  },
    };
    std::vector<std::shared_ptr<Triangle>> list;
    std::shared_ptr<Triangle> tr1 = std::shared_ptr<Triangle>(new Triangle(quadVertices[0], quadVertices[1], quadVertices[2]));
    std::shared_ptr<Triangle> tr2 = std::shared_ptr<Triangle>(new Triangle(quadVertices[0], quadVertices[2], quadVertices[3]));
    tr1->SetMaterial(material);
    tr2->SetMaterial(material);
    list.push_back(tr1);
    list.push_back(tr2);
    return std::shared_ptr<TriangleMesh>(new TriangleMesh(list, transform));
}

void TriangleMesh::transform(glm::mat4 transform) {
    auto normalTransfrom = glm::transpose(glm::inverse(transform));
    for (auto& t : _triangles) {
        t->ApplyTransform(transform, normalTransfrom);
    }
}
