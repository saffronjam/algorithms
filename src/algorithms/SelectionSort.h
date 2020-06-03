#pragma once

#include "IAlgorithm.h"

class SelectionSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Selection Sort"; }
};