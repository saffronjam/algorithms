#include "SelectionSort.h"

namespace Se
{
SelectionSort::SelectionSort() : Algorithm("Selection Sort")
{
}

void SelectionSort::Sort()
{
    for (size_t i = 0; i < GetElements().size() - 1 && _state != State::BeingCollected; i++)
    {
        PauseCheck();
        size_t indexFound = i;
        SetColor(i, sf::Color::Blue);
        for (size_t j = i + 1; j < GetElements().size() && _state != State::BeingCollected; j++)
        {
            PauseCheck();
            SetColor(j, sf::Color::Yellow);
            if (GetValue(j) < GetValue(indexFound))
            {
                indexFound = j;
                SetColor(indexFound, sf::Color::Red);
            }
            SleepDelay();
        }
        SwapElements(i, indexFound);
        for (size_t j = i + 1; j < GetElements().size(); j++)
        {
            SetColor(j, sf::Color::White);
        }
        SetColor(i, sf::Color::Green);
    }
}
}