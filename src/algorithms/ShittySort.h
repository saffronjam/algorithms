#pragma once

#include "IAlgorithm.h"
#include "Random.h"

class ShittySort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Shitty Sort"; }
};