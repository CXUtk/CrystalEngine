#include "Accelerator.h"
#include "KDTree.h"
std::unique_ptr<Accelerator> Accelerator::GetAccelerator(const std::string& name) {
    return std::make_unique<KDTree>();
}
