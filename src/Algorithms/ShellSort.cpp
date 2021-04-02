#include "ShellSort.h"

namespace Se
{
ShellSort::ShellSort() :
	Algorithm("Shell Sort")
{
}

void ShellSort::Sort()
{
	for (size_t gap = GetElements().size() / 2; gap > 0 && _state != State::BeingCollected; gap /= 2)
	{
		for (size_t i = gap; i < GetElements().size() && _state != State::BeingCollected; i++)
		{
			SetColor(i, sf::Color::Red);
			PauseCheck();
			const Element tmp = GetElement(i);
			size_t j;
			for (j = i; j >= gap && GetValue(j - gap) > tmp.value && _state != State::BeingCollected; j -= gap)
			{
				SetColor(j, sf::Color::Red);
				SetColor(j - gap, sf::Color::Red);
				SetValue(j, GetValue(j - gap));
				PauseCheck();
				SleepDelay();
				SetColor(j, sf::Color::White);
				SetColor(j - gap, sf::Color::White);
			}
			SleepDelay();
			SetValue(j, tmp.value);
			SetColor(i, sf::Color::White);
			SetColor(j, sf::Color::White);
		}
	}
}
}
