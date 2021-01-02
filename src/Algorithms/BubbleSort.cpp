#include "BubbleSort.h"

namespace Se
{

BubbleSort::BubbleSort() : Algorithm("Bubble Sort")
{
}

void BubbleSort::Sort()
{
    _state = State::Sorting;
    for (size_t i = 0; i < GetElements().size() && _state != State::BeingCollected; i++)
    {
        for (size_t j = 0; j < GetElements().size() - i - 1 && _state != State::BeingCollected; j++)
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
        SetColor(GetElements().size() - i - 1, sf::Color::Green);
    }
}

}