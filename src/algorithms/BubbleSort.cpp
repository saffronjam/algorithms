#include "BubbleSort.h"

void BubbleSort::Sort()
{
    m_state = State::Sorting;
    for (size_t i = 0; i < GetContainer().size() && m_state != State::BeingCollected; i++)
    {
        for (size_t j = 0; j < GetContainer().size() - i - 1 && m_state != State::BeingCollected; j++)
        {
            PauseCheck();
            SetColor(j, sf::Color::Red);
            SetColor(j + 1, sf::Color::Red);

            if (GetValue(j) > GetValue(j + 1))
            {
                SwapElements(j, j + 1);
            }
            SleepDelay();

            SetColor(j, sf::Color::White);
            SetColor(j + 1, sf::Color::White);
        }
        SetColor(GetContainer().size() - i - 1, sf::Color::Green);
    }
}