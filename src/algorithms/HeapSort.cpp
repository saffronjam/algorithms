#include "HeapSort.h"

void HeapSort::Sort()
{
    for (long i = (long)m_bars.size() / 2 - 1; i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        Heapify(m_bars.size(), i);
    }

    for (long i = (long)m_bars.size() - 1; i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        std::swap(m_bars[i], m_bars[0]);
        m_bars[i].color = sf::Color::Green;
        Heapify(i, 0);
        for (long j = i; j >= 0; j--)
        {
            m_bars[j].color = sf::Color::White;
        }
        m_bars[i].color = sf::Color::Green;
    }
}

void HeapSort::Heapify(const size_t &maxSizeCheck, const size_t &parent)
{
    size_t largest = parent; //assume parent is largest
    const size_t leftChild = 2 * parent + 1;
    const size_t rightChild = 2 * parent + 2;

    if (leftChild < maxSizeCheck && m_state != State::BeingCollected)
    {
        m_bars[leftChild].color = sf::Color::Red;
        if (m_bars[leftChild].value > m_bars[largest].value)
        {
            largest = leftChild;
        }
        SleepDelay();
    }

    if (rightChild < maxSizeCheck && m_state != State::BeingCollected)
    {
        m_bars[rightChild].color = sf::Color::Red;
        if (m_bars[rightChild].value > m_bars[largest].value)
        {
            largest = rightChild;
        }
        SleepDelay();
    }

    if (largest != parent && m_state != State::BeingCollected)
    {
        PauseCheck();
        std::swap(m_bars[parent], m_bars[largest]);
        Heapify(maxSizeCheck, largest);
    }
}