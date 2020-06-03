#pragma once

#include "IAlgorithm.h"

class InsertionSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Insertion Sort"; }
};