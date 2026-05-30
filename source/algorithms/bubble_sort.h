#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class BubbleSort : public Algorithm
{
public:
	BubbleSort();

	void Sort() override;
};
}
