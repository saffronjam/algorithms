#pragma once

#include "Sort.hpp"

class MergeSort : public Sort
{
public:
    MergeSort(std::string const& sortName);

private:
    void Step() override;
    void Split(BarIter start, BarIter end);
    void Merge(BarIter leftStart, BarIter leftEnd, BarIter rightStart, BarIter rightEnd);
};