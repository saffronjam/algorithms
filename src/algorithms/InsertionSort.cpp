#include "InsertionSort.h"

void InsertionSort::Sort()
{
    for (size_t i = 1; i < GetContainer().size() && m_state != State::BeingCollected; i++)
    {
        for (size_t j = i; j > 0 && m_state != State::BeingCollected; j--)
        {
            PauseCheck();
            if (GetValue(j) < GetValue(j - 1))
            {
                SetColor(j, sf::Color::Red);
                SetColor(j - 1, sf::Color::Yellow);
                SleepDelay();
                SwapElements(j, j - 1);
                SetColor(j, sf::Color::White);
            }
            else
            {
                SleepDelay();
                break;
            }
        }
        for (int j = i; j >= 0; j--)
        {
            SetColor(j, sf::Color::Cyan);
        }
    }
}