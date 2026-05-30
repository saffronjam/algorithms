#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class QuickSort : public Algorithm
{
public:
	QuickSort();

	void Sort() override;

private:
	long Partition(long start, long end);

	void Quick(long start, long end);
};
}
