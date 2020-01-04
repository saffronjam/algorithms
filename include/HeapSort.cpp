#include "HeapSort.hpp"

#include "Math.hpp"

HeapSort::HeapSort(std::string const &sortName)
    : Sort(sortName)
{
}

void HeapSort::Step()
{
    for (long i = (long)m_allBars->size() / 2 - 1; i >= 0 && !m_isBeingCollected; i--)
    {
        ContinueIfNotPaused();
        Heapify(m_allBars->size(), i);
    }

    for (long i = (long)m_allBars->size() - 1; i >= 0 && !m_isBeingCollected; i--)
    {
        ContinueIfNotPaused();
        std::swap((*m_allBars)[i], (*m_allBars)[0]);
        Heapify(i, 0);
        for (long j = i; j >= 0; j--)
        {
            (*m_allBars)[j]->SetColor(sf::Color::White);
        }
    }
}

void HeapSort::Heapify(size_t const &maxSizeCheck, size_t const &parent)
{
    size_t largest = parent; //assume parent is largest
    const size_t leftChild = 2 * parent + 1;
    const size_t rightChild = 2 * parent + 2;

    if (leftChild < maxSizeCheck && !m_isBeingCollected)
    {
        (*m_allBars)[leftChild]->SetColor(sf::Color::Red);
        if ((*m_allBars)[leftChild]->GetValue() > (*m_allBars)[largest]->GetValue())
        {
            largest = leftChild;
        }
        SleepDelay();
    }

    if (rightChild < maxSizeCheck && !m_isBeingCollected)
    {
        (*m_allBars)[rightChild]->SetColor(sf::Color::Red);
        if ((*m_allBars)[rightChild]->GetValue() > (*m_allBars)[largest]->GetValue())
        {
            largest = rightChild;
        }
        SleepDelay();
    }

    if (largest != parent && !m_isBeingCollected)
    {
        ContinueIfNotPaused();
        std::swap((*m_allBars)[parent], (*m_allBars)[largest]);
        Heapify(maxSizeCheck, largest);
    }
}