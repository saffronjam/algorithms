#pragma once

#include <Saffron.h>

#include "Algorithm.h"

namespace Se
{
class GnomeSort : public Algorithm
{
public:
    GnomeSort();

    void Sort() override;
};

}