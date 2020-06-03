#include "HeapSort.h"

void HeapSort::Sort()
{
    for (long i = (long)m_elements.size() / 2 - 1; i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        Heapify(m_elements.size(), i);
    }

    for (long i = (long)m_elements.size() - 1; i >= 0 && m_state != State::BeingCollected; i--)
    {
        PauseCheck();
        std::swap(m_elements[i], m_elements[0]);
        m_elements[i].color = sf::Color::Green;
        Heapify(i, 0);
        for (long j = i; j >= 0; j--)
        {
            m_elements[j].color = sf::Color::White;
        }
        m_elements[i].color = sf::Color::Green;
    }
}

void HeapSort::Heapify(const size_t &maxSizeCheck, const size_t &parent)
{
    size_t largest = parent; //assume parent is largest
    const size_t leftChild = 2 * parent + 1;
    const size_t rightChild = 2 * parent + 2;

    if (leftChild < maxSizeCheck && m_state != State::BeingCollected)
    {
        m_elements[leftChild].color = sf::Color::Red;
        if (m_elements[leftChild].value > m_elements[largest].value)
        {
            largest = leftChild;
        }
        SleepDelay();
    }

    if (rightChild < maxSizeCheck && m_state != State::BeingCollected)
    {
        m_elements[rightChild].color = sf::Color::Red;
        if (m_elements[rightChild].value > m_elements[largest].value)
        {
            largest = rightChild;
        }
        SleepDelay();
    }

    if (largest != parent && m_state != State::BeingCollected)
    {
        PauseCheck();
        std::swap(m_elements[parent], m_elements[largest]);
        Heapify(maxSizeCheck, largest);
    }
}