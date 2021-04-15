#include "PMKDTree.h"

PMKDTree::PMKDTree() : _root(0), _tot(0) {
}

PMKDTree::~PMKDTree() {
}

void PMKDTree::Clear() {
    _root = _tot = 0;
    _nodes.clear();
    _photons.clear();
}

void PMKDTree::Build(const std::vector<Photon>& objects) {
    _photons = objects;
    _nodes.reserve(objects.size() * 4);
    _nodes.push_back(PMKDTreeNode());
    _build(_root, 0, _photons.size() - 1);
}

std::vector<Photon*> PMKDTree::NearestNeighbor(const glm::vec3& pos, int K) {
    std::priority_queue<QNode> Q;
    std::vector<Photon*> result;
    for (int i = 0; i < K; i++) Q.push(QNode(std::numeric_limits<float>::infinity(), -1));
    _query(_root, pos, Q, K);
    while (!Q.empty()) {
        auto& node = Q.top();
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
    std::nth_element(_photons.begin() + l, _photons.begin() + mid, _photons.begin() + r + 1, cmp);
    p = newNode(_photons[mid], split);

    _build(_nodes[p].ch[0], l, mid - 1);
    _build(_nodes[p].ch[1], mid + 1, r);

    _nodes[p].PushUp(_nodes[_nodes[p].ch[0]], _nodes[_nodes[p].ch[1]]);
}

void PMKDTree::_query(int p, const glm::vec3& pos, std::priority_queue<QNode>& Q, int K) {
    if (!p) return;
    auto& node = _nodes[p];
    auto len = glm::length(pos - node.photon.Pos);
    if (Q.top().distance > len) {
        Q.pop();
        Q.push(QNode(len, p));
    }
    auto split = node.splitAxis;
    int d = pos[split] > node.photon.Pos[split];
    auto& box = _nodes[node.ch[d]].box;
    len = Q.top().distance;
    auto predict = std::max({ 0.f, box.GetMinPos()[split] - pos[split], pos[split] - box.GetMaxPos()[split] });
    if (predict < len)
        _query(node.ch[d], pos, Q, K);

    len = Q.top().distance;

    if (!node.ch[!d]) return;
    auto& box2 = _nodes[node.ch[!d]].box;
    predict = (!d ? (box2.GetMinPos()[split] - pos[split]) : (pos[split] - box2.GetMaxPos()[split]));
    if (predict < len)
        _query(node.ch[!d], pos, Q, K);

}
