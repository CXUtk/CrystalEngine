﻿#include "Accelerator.h"
#include "KDTree.h"
#include "Brute.h"
std::unique_ptr<Accelerator> Accelerator::GetAccelerator(const std::string& name) {
    if (name == "Brute") {
        return std::make_unique<Brute>();
    }
    return std::make_unique<KDTree>();
}
