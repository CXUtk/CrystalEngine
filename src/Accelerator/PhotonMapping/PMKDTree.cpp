#include "PMKDTree.h"

PMKDTree::PMKDTree() {
}

PMKDTree::~PMKDTree() {
}

void PMKDTree::Build(const std::vector<Photon>& objects) {
    _photons = objects;
    _nodes.reserve(objects.size());
    _nodes.push_back(PMKDTreeNode());
    _build(_root, 0, _photons.size() - 1);
}

std::vector<Photon*> PMKDTree::NearestNeighbor(const glm::vec3& pos, int K) {
    std::priority_queue<QNode> Q;
    std::vector<Photon*> result;
    for (int i = 0; i < K; i++) Q.push(QNode(std::numeric_limits<float>::infinity(), -1));
    _query(_root, pos, Q, K);
    while (!Q.empty()) {
        auto node = Q.top();
        Q.pop();
        result.push_back(&_nodes[node.id].photon);
    }
    return result;
}

int PMKDTree::newNode(const Photon& photon, int split) {
    _tot++;
    _nodes.push_back(PMKDTreeNode(photon, split));
    return _tot;
}

void PMKDTree::_build(int& p, int l, int r) {
    if (l > r) return;
    int mid = (l + r) / 2;
    glm::vec3 minn = _photons[l].Pos, maxx = _photons[r].Pos;
    for (int i = l + 1; i < r; i++) {
        minn = glm::min(minn, _photons[i].Pos);
        maxx = glm::max(maxx, _photons[i].Pos);
    }
    int split = 0;
    for (int i = 1; i < 3; i++) {
        if (maxx[i] - minn[i] > maxx[split] - minn[split]) {
            split = i;
        }
    }
    if (l == r) {
        p = newNode(_photons[mid], split);
        return;
    }
    auto cmp = [=](const Photon& a, const Photon& b) {
        return a.Pos[split] < b.Pos[split];
    };
    std::nth_element(_photons.begin() + l, _photons.begin() + mid, _photons.end());
    p = newNode(_photons[mid], split);

    _build(_nodes[p].ch[0], l, mid - 1);
    _build(_nodes[p].ch[1], mid + 1, r);

    _nodes[p].PushUp(_nodes[_nodes[p].ch[0]], _nodes[_nodes[p].ch[1]]);
}

void PMKDTree::_query(int p, const glm::vec3& pos, std::priority_queue<QNode>& Q, int K) {
    if (!p) return;
    auto node = _nodes[p];
    auto len = glm::length(pos - node.photon.Pos);
    if (Q.top().distance > len) {
        Q.pop();
        Q.push(QNode(len, p));
    }
    len = Q.top().distance;
    float nearest = len + 1;
    auto box = _nodes[node.ch[0]].box;
    for (int i = 0; i < 3; i++) {
        nearest = std::min(nearest, std::abs(pos[i] - box.GetMaxPos()[i]));
        nearest = std::min(nearest, std::abs(pos[i] - box.GetMinPos()[i]));
    }
    if (nearest < len) _query(node.ch[0], pos, Q, K);

    len = Q.top().distance;
    nearest = len + 1;
    box = _nodes[node.ch[1]].box;
    for (int i = 0; i < 3; i++) {
        nearest = std::min(nearest, std::abs(pos[i] - box.GetMaxPos()[i]));
        nearest = std::min(nearest, std::abs(pos[i] - box.GetMinPos()[i]));
    }
    if (nearest < len) _query(node.ch[1], pos, Q, K);
}
