#include "GnomeSort.h"

void GnomeSort::Sort()
{
    for (int i = 0; i < GetContainer().size() && m_state != State::BeingCollected;)
    {
        if (i == 0)
        {
            SetColor(i, sf::Color::Cyan);
            i++;
        }
        if (GetValue(i) >= GetValue(i - 1) && m_state != State::BeingCollected)
        {
            PauseCheck();
            SetColor(i, sf::Color::Cyan);
            i++;
        }
        else
        {
            SwapElements(i, i - 1);
            SetColor(i, sf::Color::Red);
            SetColor(i - 1, sf::Color::Red);
            PauseCheck();
            SetColor(i, sf::Color::White);
            i--;
        }
        SleepDelay();
    }
}