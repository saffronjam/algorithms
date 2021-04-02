#include "RadixSort.h"

namespace Se
{
RadixSort::RadixSort() :
	Algorithm("Radix Sort")
{
}

void RadixSort::Sort()
{
	const long h = HighestValue();
	for (size_t exp = 1; static_cast<size_t>(h / exp) > 0 && _state != State::BeingCollected; exp *= 10) CountSort(exp);
}

void RadixSort::CountSort(size_t exponent)
{
	ArrayList<Element> outBucket(GetElements().size());
	std::array<size_t, 10> count = {0};

	// Store count of occurrences in count[]
	for (size_t i = 0; i < GetElements().size() && _state != State::BeingCollected; i++)
	{
		PauseCheck();
		count[(GetValue(i) / exponent) % 10]++;
	}

	// Change count[i] so that count[i] now contains actual
	//  position of this digit in output[]
	for (size_t i = 1; i < 10 && _state != State::BeingCollected; i++)
	{
		PauseCheck();
		count[i] += count[i - 1];
	}

	// Build the output array
	for (long i = static_cast<long>(GetElements().size() - 1); i >= 0 && _state != State::BeingCollected; i--)
	{
		PauseCheck();
		outBucket[count[(GetValue(i) / exponent) % 10] - 1] = GetElement(i);
		count[(GetValue(i) / exponent) % 10]--;
	}

	// Copy the output array to arr[], so that arr[] now
	// contains sorted numbers according to current digit
	for (size_t i = 0; i < GetElements().size() && _state != State::BeingCollected; i++)
	{
		SetColor(i, sf::Color::Red);
		SleepDelay();
		SleepDelay();
		SleepDelay();
		PauseCheck();
		SetValue(i, outBucket[i].value);
		SetColor(i, sf::Color::White);
	}

	if (_state != State::BeingCollected) GetElements() = outBucket;
}

long RadixSort::HighestValue()
{
	return std::max_element(GetElements().begin(), GetElements().end(), [](const Element& first, const Element& second)
	{
		return first.value < second.value;
	})->value;
}
}
