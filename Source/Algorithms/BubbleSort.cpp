#include "BubbleSort.h"

namespace Se
{
BubbleSort::BubbleSort() :
	Algorithm("Bubble Sort")
{
}

void BubbleSort::Sort()
{
	_state = State::Sorting;
	for (size_t i = 0; i < Elements().size() && _state != State::BeingCollected; i++)
	{
		for (size_t j = 0; j < Elements().size() - i - 1 && _state != State::BeingCollected; j++)
		{
			PauseCheck();
			SetColor(j, sf::Color::Red);
			SetColor(j + 1, sf::Color::Red);

			if (ValueByIndex(j) > ValueByIndex(j + 1))
			{
				SwapElements(j, j + 1);
			}
			SleepDelay();

			SetColor(j, sf::Color::White);
			SetColor(j + 1, sf::Color::White);
		}
		SetColor(Elements().size() - i - 1, sf::Color::Green);
	}
}
}
