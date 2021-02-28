#pragma once
#include "Accelerator.h"

class Brute : public Accelerator {
public:
    Brute();
    ~Brute() override;
    void Build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool Intersect(const Ray& ray, HitRecord* info) const override;

private:
    std::vector<Object*> _objects;
};
