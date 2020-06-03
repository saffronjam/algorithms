#pragma once

#include "IAlgorithm.h"

class MergeSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Merge Sort"; }

private:
    void Split(size_t iStart, size_t iEnd);
    void Merge(size_t iLeftStart, size_t iLeftEnd, size_t iRightStart, size_t iRightEnd);
};