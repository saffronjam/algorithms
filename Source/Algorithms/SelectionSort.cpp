#include "SelectionSort.h"

namespace Se
{
SelectionSort::SelectionSort() :
	Algorithm("Selection Sort")
{
}

void SelectionSort::Sort()
{
	for (size_t i = 0; i < Elements().size() - 1 && _state != State::BeingCollected; i++)
	{
		PauseCheck();
		size_t indexFound = i;
		SetColor(i, sf::Color::Blue);
		for (size_t j = i + 1; j < Elements().size() && _state != State::BeingCollected; j++)
		{
			PauseCheck();
			SetColor(j, sf::Color::Yellow);
			if (ValueByIndex(j) < ValueByIndex(indexFound))
			{
				indexFound = j;
				SetColor(indexFound, sf::Color::Red);
			}
			SleepDelay();
		}
		SwapElements(i, indexFound);
		for (size_t j = i + 1; j < Elements().size(); j++)
		{
			SetColor(j, sf::Color::White);
		}
		SetColor(i, sf::Color::Green);
	}
}
}
