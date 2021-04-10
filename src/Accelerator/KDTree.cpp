#include "KDTree.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

struct Edge {
    bool end;
    float pos;
    Edge(bool end, float pos) : end(end), pos(pos) {}
    bool operator<(const Edge& edge) const {
        if (pos == edge.pos)return end > edge.end;
        return pos < edge.pos;
    }
};

KDTree::KDTree() : Accelerator() {
    _nodes.push_back(KDTreeNode());
    _root = 0, _tot = 0;
    _objects.clear();
}

KDTree::~KDTree() {
}

void KDTree::Build(const std::vector<std::shared_ptr<Object>>& objects) {
    masterBox = BoundingBox();
    _nodes.reserve(objects.size() * 100);
    for (const auto& ptr : objects) {
        _objects.push_back(ptr.get());
        masterBox = masterBox.Union(ptr->GetBoundingBox());
    }
    int sz = _objects.size();
    _build(_root, masterBox, _objects, 0);
}

bool KDTree::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    bool ret = false;
    float tMin = 0, tMax = info->GetDistance();
    if (!masterBox.RayIntersects(ray, tMin, tMax)) return false;
    ret |= ray_test(_root, ray, info, tMin, tMax);
    return ret;
}

bool KDTree::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    return ray_test_p(_root, ray, tMin, tMax);
}




int KDTree::newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split, float splitPos) {
    ++_tot;
    _nodes.push_back(KDTreeNode(objs, split, splitPos));
    return _tot;
}

void KDTree::push_up(int p) {
}

void KDTree::_build(int& p, const BoundingBox& outerBox, std::vector<Object*>& objs, int depth) {
    if (objs.size() <= 2 || depth == MAX_DEPTH) {
        p = newNode(objs, outerBox, -1, -1);
        return;
    }
    int split = 0;
    float splitPos = 0;
    if (SPLIT_METHOD == SplitMethod::SAH) {
        float smallestCost = std::numeric_limits<float>::infinity();
        float totalSA = outerBox.SurfaceArea();
        glm::vec3 diff = outerBox.GetMaxPos() - outerBox.GetMinPos();
        for (int d = 0; d < 3; d++) {
            std::vector<Edge> edges;
            for (auto& obj : objs) {
                edges.push_back(Edge(false, obj->GetBoundingBox().GetMinPos()[d]));
                edges.push_back(Edge(true, obj->GetBoundingBox().GetMaxPos()[d]));
            }
            int nBelow = 0, nAbove = objs.size();
            float L = outerBox.GetMinPos()[d], R = outerBox.GetMaxPos()[d];
            sort(edges.begin(), edges.end());
            for (int i = 0; i < edges.size(); i++) {
                Edge& e = edges[i];
                if (e.end) --nAbove;
                if (e.pos > L && e.pos < R) {
                    float LA = diff[(d + 1) % 3];
                    float LB = diff[(d + 2) % 3];
                    float belowSA = 2 * (LA * LB + (e.pos - L) * (LA + LB));
                    float aboveSA = 2 * (LA * LB + (R - e.pos) * (LA + LB));
                    float eb = (nAbove == 0 || nBelow == 0) ? 0.2 : 0;
                    float pb = belowSA / totalSA, pa = aboveSA / totalSA;
                    float cost = 0.05 + (1 - eb) * (pb * nBelow + pa * nAbove);
                    if (cost < smallestCost) {
                        smallestCost = cost;
                        split = d;
                        splitPos = e.pos;
                    }
                }
                if (!e.end) ++nBelow;
            }
        }
        if (smallestCost >= objs.size()) {
            p = newNode(objs, outerBox, -1, -1);
            return;
        }
    }
    else {
        auto cmp = [=](Object* A, Object* B) {
            return A->GetBoundingBox().GetCenter()[split] < B->GetBoundingBox().GetCenter()[split];
        };
        split = outerBox.MaxExtent();
        int mid = objs.size() / 2;
        std::nth_element(objs.begin(), objs.begin() + mid, objs.end(), cmp);
        splitPos = objs[mid]->GetBoundingBox().GetCenter()[split];
    }
    glm::vec3 leftM = outerBox.GetMaxPos();
    leftM[split] = splitPos;

    glm::vec3 rightM = outerBox.GetMinPos();
    rightM[split] = splitPos;

    BoundingBox boxleft(outerBox.GetMinPos(), leftM);
    BoundingBox boxright(rightM, outerBox.GetMaxPos());

    std::vector<Object*> leftTri, rightTri;
    for (auto&
        obj : objs) {
        if (obj->GetBoundingBox().GetMinPos()[split] <= splitPos) {
            leftTri.push_back(obj);
        }
        if (obj->GetBoundingBox().GetMaxPos()[split] > splitPos) {
            rightTri.push_back(obj);
        }
    }

    p = newNode(objs, outerBox, split, splitPos);
    _build(chi(p, 0), boxleft, leftTri, depth + 1);
    _build(chi(p, 1), boxright, rightTri, depth + 1);
    push_up(p);
}

bool KDTree::ray_test(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const {
    if (!p || tMin > tMax) return false;
    //if (!outerBox.rayIntersect(ray, tMin, tMax)) return false;
    if (tMin >= info->GetDistance()) return false;
    if (_nodes[p].splitAxis == -1) {
        bool hit = false;
        for (auto& obj : _nodes[p].objs) {
            SurfaceInteraction tmp;
            if (obj->Intersect(ray, &tmp)) {
                if (tmp.GetDistance() < info->GetDistance()) {
                    *info = tmp;
                }
                hit = true;
            }
        }
        return hit;
    }
    int split = self.splitAxis;
    float splitPos = self.splitPos;
    float tSplit = (splitPos - ray.start[split]) / ray.dir[split];
    bool hit = false;
    int d = ray.dir[split] < 0;
    if (tSplit >= tMin) {
        hit |= ray_test(chi(p, d), ray, info, tMin, std::min({ tSplit, tMax, info->GetDistance() }));
    }
    if (tSplit <= tMax) {
        hit |= ray_test(chi(p, !d), ray, info, std::max(tSplit, 0.0f), std::min(tMax, info->GetDistance()));
    }
    return hit;
}

bool KDTree::ray_test_p(int p, const Ray& ray, float tMin, float tMax) const {
    if (!p || tMin > tMax) return false;
    //if (!outerBox.rayIntersect(ray, tMin, tMax)) return false;
    if (_nodes[p].splitAxis == -1) {
        for (auto& obj : _nodes[p].objs) {
            if (obj->IntersectTest(ray, tMin, tMax)) {
                return true;
            }
        }
        return false;
    }
    int split = self.splitAxis;
    float splitPos = self.splitPos;
    float tSplit = (splitPos - ray.start[split]) / ray.dir[split];
    bool hit = false;
    int d = ray.dir[split] < 0;
    if (tSplit >= tMin) {
        hit |= ray_test_p(chi(p, d), ray, tMin, std::min({ tSplit, tMax }));
    }
    if (hit) return true;
    if (tSplit <= tMax) {
        hit |= ray_test_p(chi(p, !d), ray, std::max(tSplit, 0.0f), tMax);
    }
    return hit;
}
