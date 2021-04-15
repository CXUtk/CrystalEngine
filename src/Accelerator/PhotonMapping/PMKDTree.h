#pragma once
#include <Utils/BoundingBox.h>
#include <Core/Object.h>
#include <vector>
#include <queue>

struct Photon {
    glm::vec3 Pos;
    glm::vec3 Dir;
    glm::vec3 Power;
};
struct PMKDTreeNode {
    Photon photon;
    BoundingBox box;
    int ch[2], splitAxis;

    PMKDTreeNode() : splitAxis(-1) {
        ch[0] = ch[1] = 0;
    }

    PMKDTreeNode(const Photon& photon, int split) : photon(photon),
        splitAxis(split) {
        ch[0] = ch[1] = 0;
        box = BoundingBox(photon.Pos, photon.Pos);
    }

    void PushUp(const PMKDTreeNode& left, const PMKDTreeNode& right) {
        box = box.Union(left.box);
        box = box.Union(right.box);
    }
};


struct QNode {
    float distance;
    int id;
    QNode(float dis, int id) : distance(dis), id(id) {}
    bool operator<(const QNode& node) const {
        return distance < node.distance || (distance == node.distance && id < node.id);
    }
};

class PMKDTree {
public:
    PMKDTree();
    ~PMKDTree();
    void Clear();
    void Build(const std::vector<Photon>& objects);
    std::vector<Photon*> NearestNeighbor(const glm::vec3& pos, int K);

private:

    int _tot, _root;
    std::vector<PMKDTreeNode> _nodes;
    std::vector<Photon> _photons;

    int newNode(const Photon& photon, int split);
    void _build(int& p, int l, int r);
    void _query(int p, const glm::vec3& pos, std::priority_queue <QNode>& Q, int K);
};
