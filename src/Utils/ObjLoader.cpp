#include "ObjLoader.h"
#include <map>
#include <glm/gtx/transform.hpp>
char ObjLoader::lineBuffer[MAX_BUFFER];


struct cmpVec3 {
    bool operator()(const glm::vec3& A, const glm::vec3& B) const {
        for (int i = 0; i < 3; i++) {
            if (A[i] != B[i]) {
                if (A[i] < B[i]) return true;
                else return false;
            }
        }
        return false;
    }
};
static std::map<glm::vec3, int, cmpVec3> pointSet;
static std::map<int, int> vIDMap;
void ObjLoader::load(const std::string& path) {
    Vertices.clear();
    Triangles.clear();
    pointSet.clear();
    vIDMap.clear();
    _totV = 0;
    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        std::cerr << "Cannot open file " << path << std::endl;
        return;
    }
    while (fgets(lineBuffer, MAX_BUFFER, file)) {
        _ptr = 0;
        process();
    }
    fclose(file);
}

bool readInt(const char* S, int& idx, int& num) {
    num = 0;
    while (S[idx] == ' ') idx++;
    bool neg = false;
    if (S[idx] == '-') {
        neg = true;
        idx++;
    }
    else if (S[idx] == '+') {
        idx++;
    }
    while (isdigit(S[idx])) {
        num *= 10;
        num += S[idx] - '0';
        idx++;
    }
    if (neg) num *= -1;
    return true;
}


std::shared_ptr<TriangleMesh> ObjLoader::GetMesh(std::shared_ptr<Material> material, glm::mat4 transform) const {

    std::vector<std::shared_ptr<Triangle>> triangles;
    for (auto& t : Triangles) {
        VertexData v[3];
        v[0].Position = Vertices[t.VertexID[0]];
        v[1].Position = Vertices[t.VertexID[1]];
        v[2].Position = Vertices[t.VertexID[2]];

        if (Normals.size()) {
            v[0].Normal = Normals[t.NormalID[0]];
            v[1].Normal = Normals[t.NormalID[1]];
            v[2].Normal = Normals[t.NormalID[2]];
        }

        if (TexCoords.size()) {
            v[0].TexCoords = TexCoords[t.TexID[0]];
            v[1].TexCoords = TexCoords[t.TexID[1]];
            v[2].TexCoords = TexCoords[t.TexID[2]];
        }

        std::shared_ptr<Triangle> tri = std::shared_ptr<Triangle>(
            new Triangle(v[0], v[1], v[2])
            );
        tri->SetMaterial(material);
        triangles.push_back(tri);
    }
    return std::make_shared<TriangleMesh>(triangles, transform);
}

//std::vector<DrawTriangle> ObjLoader::GetDrawTriangles() const {
//    std::vector<DrawTriangle> triangles;
//    for (auto face : Triangles) {
//        DrawTriangle tri;
//        DrawVertex v1[3];
//        for (int i = 0; i < 3; i++) {
//            v1[i].pos = Vertices[face.v[i]];
//            v1[i].normal = Normals[face.v[i]];
//            tri.V[i] = v1[i];
//        }
//
//        triangles.push_back(tri);
//    }
//    return triangles;
//}



void ObjLoader::process() {
    static char start[16];
    static char faceV[105];
    sscanf(lineBuffer + _ptr, "%s", start);
    _ptr += strlen(start);

    if (!strcmp(start, "v")) {
        double x, y, z;
        _ptr += sscanf(lineBuffer + _ptr, "%lf%lf%lf", &x, &y, &z);
        _totV++;
        auto pt = glm::vec3(x, y, z);
        if (pointSet.find(pt) == pointSet.end()) {
            Vertices.push_back(pt);
            pointSet[pt] = Vertices.size();
            vIDMap[_totV] = Vertices.size();
        }
        else {
            vIDMap[_totV] = pointSet[pt];
        }
    }
    else if (!strcmp(start, "f")) {
        int c = 0;
        std::vector<std::tuple<int, int, int>> vertices;
        while (~(c = sscanf(lineBuffer + _ptr, "%s", faceV))) {
            int id = 0;
            int vd[3];
            memset(vd, 0, sizeof(vd));
            for (int j = 0; j < 3; j++) {
                bool b = readInt(faceV, id, vd[j]);
                id++;
                if (!b || faceV[id - 1] != '/')break;
            }
            vertices.push_back({ vIDMap[vd[0]] - 1, vIDMap[vd[1]] - 1, vIDMap[vd[2]] - 1 });
            //int i = 0;
            //int num = 0;
            //bool neg = false;
            //for (; faceV[i] != '\0' && faceV[i] != '/'; i++) {
            //    if (!isdigit(faceV[i])) {
            //        if (faceV[i] == '-') {
            //            neg = true;
            //        }
            //        continue;
            //    }
            //    // assert(isdigit(faceV[i]));
            //    num *= 10;
            //    num += faceV[i] - '0';
            //}
            //if (!neg)
            //    num--;
            //else
            //    num = Vertices.size() - num - 1;
            //vertices.push_back(num);
            _ptr += strlen(faceV) + 1;
        }

        // Triangulation process
        auto cmp = [&](int a, int b) {
            return Vertices[a].x < Vertices[b].x;
        };
        int sz = vertices.size();
        if (sz == 3) {
            Triangles.push_back(TriangleFaceIndex(vertices));
        }
        else {
            std::cerr << "Invalid obj file format" << std::endl;
            assert(false);
            return;
        }
    }
    else if (!strcmp(start, "vt")) {
        double x, y;
        _ptr += sscanf(lineBuffer + _ptr, "%lf%lf", &x, &y);
        TexCoords.push_back(glm::vec2(x, y));
    }
    else if (!strcmp(start, "vn")) {
        double x, y, z;
        _ptr += sscanf(lineBuffer + _ptr, "%lf%lf%lf", &x, &y, &z);
        Normals.push_back(glm::vec3(x, y, z));
    }
}
