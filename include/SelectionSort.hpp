#pragma once

#include "Sort.hpp"

class SelectionSort : public Sort
{
public:
    SelectionSort(std::string const &sortName);

private:
    void Step() override;
};