#pragma once

#include "IAlgorithm.h"

class ShellSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Shell Sort"; }
};