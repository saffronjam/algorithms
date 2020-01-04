#pragma once

#include "Sort.hpp"

class HeapSort : public Sort
{

public:
    HeapSort(std::string const &sortName);

private:
    void Step() override;
    void Heapify(size_t const &maxSizeCheck, size_t const &topIndex);
};