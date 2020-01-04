#pragma once

#include "Sort.hpp"

class QuickSort : public Sort
{
public:
    QuickSort(std::string const &sortName);

private:
    void Step() override;
    // BarIter Partition(BarIter start, BarIter end);
    // void Quick(BarIter start, BarIter end);

    int Partition(int start, int end);
    void Quick(int start, int end);
};