#include "ShellSort.h"

namespace Se
{
ShellSort::ShellSort() :
	Algorithm("Shell Sort")
{
}

void ShellSort::Sort()
{
	for (size_t gap = Elements().size() / 2; gap > 0 && _state != State::BeingCollected; gap /= 2)
	{
		for (size_t i = gap; i < Elements().size() && _state != State::BeingCollected; i++)
		{
			SetColor(i, sf::Color::Red);
			PauseCheck();
			const Element tmp = ElementByIndex(i);
			size_t j;
			for (j = i; j >= gap && ValueByIndex(j - gap) > tmp.Value && _state != State::BeingCollected; j -= gap)
			{
				SetColor(j, sf::Color::Red);
				SetColor(j - gap, sf::Color::Red);
				SetValue(j, ValueByIndex(j - gap));
				PauseCheck();
				SleepDelay();
				SetColor(j, sf::Color::White);
				SetColor(j - gap, sf::Color::White);
			}
			SleepDelay();
			SetValue(j, tmp.Value);
			SetColor(i, sf::Color::White);
			SetColor(j, sf::Color::White);
		}
	}
}
}
