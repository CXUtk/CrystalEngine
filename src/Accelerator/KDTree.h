#pragma once
#include <Utils/BoundingBox.h>
#include "Accelerator.h"
#include <Primitives/Object.h>
#include <vector>

struct KDTreeNode {
    std::vector<Object*> objs;
    int ch[2], splitAxis;
    float splitPos;

    KDTreeNode() : splitAxis(-1), splitPos(-1) {
        ch[0] = ch[1] = 0;
    }

    KDTreeNode(const std::vector<Object*>& objs, int split, float splitPos) : splitAxis(split), splitPos(splitPos) {
        ch[0] = ch[1] = 0;
        if (split == -1) {
            this->objs = objs;
        }
    }
};
class KDTree : public Accelerator {
public:
    KDTree();
    ~KDTree();
    void Build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;

private:
    enum SplitMethod {
        SAH,
        EQUAL,
    };
    static constexpr int MAX_DEPTH = 20;
    static constexpr int MAX_NODES = 1 << 15;
    static constexpr SplitMethod SPLIT_METHOD = SplitMethod::SAH;


    int _tot, _root;
    std::vector<KDTreeNode> _nodes;
    std::vector<Object*> _objects;
    BoundingBox masterBox;

    int newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split, float splitPos);
    void push_up(int p);
    void _build(int& p, const BoundingBox& outerBox, std::vector<Object*>& objs, int depth);
    bool ray_test(int p, const Ray& ray, HitRecord* info, float tMin, float tMax) const;
};
