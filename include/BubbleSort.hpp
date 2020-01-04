#pragma once

#include "Sort.hpp"

class BubbleSort : public Sort
{
public:
    BubbleSort(std::string const &sortName);

private:
    void Step() override;
};