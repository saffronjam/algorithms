#pragma once

#include "Sort.hpp"

class InsertionSort : public Sort
{
public:
    InsertionSort(std::string const &sortName);

private:
    void Step() override;
};