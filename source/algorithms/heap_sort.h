#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class HeapSort : public Algorithm
{
public:
	HeapSort();

	void Sort() override;

private:
	void Heapify(size_t maxSizeCheck, size_t topIndex);
};
}
