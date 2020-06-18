#pragma once

#include "IAlgorithm.h"

class HeapSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Heap Sort"; }

private:
    void Heapify(size_t maxSizeCheck, size_t topIndex);
};