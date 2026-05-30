#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class RadixSort : public Algorithm
{
public:
	RadixSort();

	void Sort() override;

private:
	void CountSort(size_t exponent);

	long HighestValue();
};
}
