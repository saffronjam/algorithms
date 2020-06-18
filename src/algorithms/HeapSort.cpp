#include "HeapSort.h"

void HeapSort::Sort()
{
    for (long i = static_cast<long>(GetContainer().size() / 2 - 1); i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        Heapify(GetContainer().size(), i);
    }

    for (long i = static_cast<long>(GetContainer().size() - 1); i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        SwapElements(i, 0);
        SetColor(i, sf::Color::Green);
        Heapify(i, 0);
        for (long j = i; j >= 0; j--)
        {
            SetColor(j, sf::Color::White);
        }
        SetColor(i, sf::Color::Green);
    }
}

void HeapSort::Heapify(size_t maxSizeCheck, size_t parent)
{
    size_t largest = parent; //assume parent is largest
    const size_t leftChild = 2 * parent + 1;
    const size_t rightChild = 2 * parent + 2;

    if (leftChild < maxSizeCheck && m_state != State::BeingCollected)
    {
        SetColor(leftChild, sf::Color::Red);
        if (GetValue(leftChild) > GetValue(largest))
        {
            largest = leftChild;
        }
        SleepDelay();
    }

    if (rightChild < maxSizeCheck && m_state != State::BeingCollected)
    {
        SetColor(rightChild, sf::Color::Red);
        if (GetValue(rightChild) > GetValue(largest))
        {
            largest = rightChild;
        }
        SleepDelay();
    }

    if (largest != parent && m_state != State::BeingCollected)
    {
        PauseCheck();
        SwapElements(parent, largest);
        Heapify(maxSizeCheck, largest);
    }
}