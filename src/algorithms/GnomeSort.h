#pragma once

#include "IAlgorithm.h"
#include "Random.h"

class GnomeSort : public IAlgorithm
{
public:
    void Sort() override;
    std::string GetName() override { return "Gnome Sort"; }
};