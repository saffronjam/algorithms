#pragma once

#include "IAlgorithm.h"

class BubbleSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Bubble Sort"; }
};