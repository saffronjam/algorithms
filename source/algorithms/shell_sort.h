#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class ShellSort : public Algorithm
{
public:
	ShellSort();

	void Sort() override;
};
}
