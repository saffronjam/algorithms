#pragma once

#include "algorithm.h"

namespace algorithms
{
using namespace saffron;
class InsertionSort : public Algorithm
{
public:
	InsertionSort();

	void Sort() override;
};
}
