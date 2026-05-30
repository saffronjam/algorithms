#pragma once

#include <saffron.h>

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class GnomeSort : public Algorithm
{
public:
	GnomeSort();

	void Sort() override;
};
}
