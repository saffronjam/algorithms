#pragma once

#include "IAlgorithm.h"

class QuickSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Quick Sort"; }

private:
    long Partition(long start, long end);
    void Quick(long start, long end);
};