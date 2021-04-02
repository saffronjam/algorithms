#include "QuickSort.h"

namespace Se
{
QuickSort::QuickSort() :
	Algorithm("Quick Sort")
{
}

void QuickSort::Sort()
{
	Quick(0, GetElements().size() - 1lu);
}

long QuickSort::Partition(long start, long end)
{
	Element* pivot = &GetElement(end);
	long iS = start - 1;

	for (long i = static_cast<long>(start); i <= end - 1 && _state != State::BeingCollected; i++)
	{
		SetColor(i, sf::Color::Blue);
	}
	for (long i = static_cast<long>(start); i <= end - 1 && _state != State::BeingCollected; i++)
	{
		PauseCheck();
		SetColor(i, sf::Color::Red);
		SleepDelay();
		if (GetValue(i) < pivot->value)
		{
			iS++;
			SwapElements(iS, i);
		}
	}
	for (long i = static_cast<long>(start); i <= end - 1 && _state != State::BeingCollected; i++)
	{
		SetColor(i, sf::Color::White);
	}
	SwapElements(iS + 1, end);
	return (iS + 1);
}

void QuickSort::Quick(long start, long end)
{
	if (start < end && _state != State::BeingCollected)
	{
		PauseCheck();
		const long iP = Partition(start, end); //partion index

		Quick(start, iP - 1);
		Quick(iP + 1, end);
	}
}
}
