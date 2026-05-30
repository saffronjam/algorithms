#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class SelectionSort : public Algorithm
{
public:
	SelectionSort();

	void Sort() override;
};
}
