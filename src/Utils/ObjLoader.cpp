﻿#include "ObjLoader.h"
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
void ObjLoader::load(const std::string& path) {
    Positions.clear();
    Triangles.clear();
    PRTs.clear();
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

int fix(int id, int maxsz) {
    if (id < 0) return maxsz + id;
    return id;
}

std::shared_ptr<TriangleMesh> ObjLoader::GetMesh(std::shared_ptr<Material> material, glm::mat4 transform) {
    glm::mat4 normalTrans = glm::transpose(glm::inverse(transform));
    int sz = Vertices.size();
    for (int i = 0; i < sz; i++) {
        VertexData& v = Vertices[i];
        v.Position = glm::vec3(transform * glm::vec4(v.Position, 1.0f));

        if (Normals.size()) {
            v.Normal = glm::vec3(normalTrans * glm::vec4(v.Normal, 0.0f));
        }
        if (!PRTs.empty()) {
            v.PRT = PRTs[i];
        }
    }
    return std::make_shared<TriangleMesh>(Vertices, Triangles, material, nullptr);
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
        Positions.push_back(pt);
    }
    else if (!strcmp(start, "f")) {
        int index = 0;
        int c = 0;
        int vd[3]{}, vn[3]{};
        while (~(c = sscanf(lineBuffer + _ptr, "%s", faceV))) {
            int id = 0;
            bool b = readInt(faceV, id, vd[index]);
            id++;
            if (faceV[id - 1] != '/')break;
            // Jump to normals
            while (faceV[id] != '/') {
                id++;
            }
            id++;
            readInt(faceV, id, vn[index]);
            index++;
            _ptr += strlen(faceV) + 1;
        }
        VertexData V[3];
        for (int k = 0; k < 3; k++) {
            V[k].Position = Positions[vd[k] - 1];
        }
        if (vn[0] != 0) {
            for (int k = 0; k < 3; k++) {
                V[k].Normal = Normals[vn[k] - 1];
            }
        }
        int cur = Vertices.size();
        Triangles.push_back(glm::ivec3(cur, cur + 1, cur + 2));
        for (int k = 0; k < 3; k++) {
            Vertices.push_back(V[k]);
        }

        //// Triangulation process
        //auto cmp = [&](int a, int b) {
        //    return Positions[a].x < Positions[b].x;
        //};
        //int sz = vertices.size();
        //if (sz == 3) {
        //    Triangles.push_back(TriangleFaceIndex(vertices));
        //}
        //else {
        //    std::cerr << "Invalid obj file format" << std::endl;
        //    assert(false);
        //    return;
        //}
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
