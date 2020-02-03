#pragma once

#include "Sort.hpp"
#include "Random.hpp"

class ShittySort : public Sort
{
public:
    ShittySort(std::string const &sortName);

private:
    void Step() override;

    void RandomizeList();
};