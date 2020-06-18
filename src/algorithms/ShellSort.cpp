#include "ShellSort.h"

void ShellSort::Sort()
{
    for (size_t gap = GetContainer().size() / 2; gap > 0 && m_state != State::BeingCollected; gap /= 2)
    {
        for (size_t i = gap; i < GetContainer().size() && m_state != State::BeingCollected; i++)
        {
            PauseCheck();
            SetColor(i, sf::Color::Red);
            const Element tmp = GetElement(i);
            size_t j;
            for (j = i; j >= gap && GetValue(j - gap) > tmp.value && m_state != State::BeingCollected; j -= gap)
            {
                SetColor(j, sf::Color::Red);
                SetColor(j - gap, sf::Color::Red);
                SetValue(j, GetValue(j - gap));
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