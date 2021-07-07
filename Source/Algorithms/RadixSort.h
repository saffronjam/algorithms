#pragma once

#include "Algorithm.h"

namespace Se
{
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
