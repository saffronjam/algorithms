#pragma once

#include "Algorithm.h"

namespace Se
{
class HeapSort : public Algorithm
{
public:
	HeapSort();

	void Sort() override;

private:
	void Heapify(size_t maxSizeCheck, size_t topIndex);
};
}
