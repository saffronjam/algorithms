#pragma once

#include "Algorithm.h"

namespace Se
{
class MergeSort : public Algorithm
{
public:
    MergeSort();

    void Sort() override;

private:
    void Split(size_t iStart, size_t iEnd);

    void Merge(size_t iLeftStart, size_t iLeftEnd, size_t iRightStart, size_t iRightEnd);
};
}