#pragma once

#include "Sort.hpp"

class RadixSort : public Sort
{
public:
    RadixSort(std::string const &sortName);

private:
    void Step() override;

    void CountSort(int const &exponent);

    double HighestValue();
};