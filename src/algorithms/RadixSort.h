#pragma once

#include "IAlgorithm.h"

class RadixSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Radix Sort"; }

private:
    void CountSort(size_t exponent);
    long HighestValue();
};