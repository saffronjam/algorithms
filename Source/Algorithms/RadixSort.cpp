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
	List<Element> outBucket(Elements().size());
	std::array<size_t, 10> count = {0};

	// Store count of occurrences in count[]
	for (size_t i = 0; i < Elements().size() && _state != State::BeingCollected; i++)
	{
		PauseCheck();
		count[(ValueByIndex(i) / exponent) % 10]++;
	}

	// Change count[i] so that count[i] now contains actual
	//  position of this digit in output[]
	for (size_t i = 1; i < 10 && _state != State::BeingCollected; i++)
	{
		PauseCheck();
		count[i] += count[i - 1];
	}

	// Build the output array
	for (long i = static_cast<long>(Elements().size() - 1); i >= 0 && _state != State::BeingCollected; i--)
	{
		PauseCheck();
		outBucket[count[(ValueByIndex(i) / exponent) % 10] - 1] = ElementByIndex(i);
		count[(ValueByIndex(i) / exponent) % 10]--;
	}

	// Copy the output array to arr[], so that arr[] now
	// contains sorted numbers according to current digit
	for (size_t i = 0; i < Elements().size() && _state != State::BeingCollected; i++)
	{
		SetColor(i, sf::Color::Red);
		SleepDelay();
		SleepDelay();
		SleepDelay();
		PauseCheck();
		SetValue(i, outBucket[i].Value);
		SetColor(i, sf::Color::White);
	}

	if (_state != State::BeingCollected) Elements() = outBucket;
}

auto RadixSort::HighestValue() -> long
{
	return std::max_element(Elements().begin(), Elements().end(), [](const Element& first, const Element& second)
	{
		return first.Value < second.Value;
	})->Value;
}
}
