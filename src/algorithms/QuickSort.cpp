#include "QuickSort.h"

void QuickSort::Sort()
{
    Quick(0, m_elements.size() - 1);
}

long QuickSort::Partition(long start, long end)
{
    Element *pivot = &m_elements[end];
    long iS = start - 1;

    for (long i = (long)start; i <= end - 1 && m_state != State::BeingCollected; i++)
    {
        m_elements[i].color = sf::Color::Blue;
    }
    for (long i = (long)start; i <= end - 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        m_elements[i].color = sf::Color::Red;
        SleepDelay();
        if (m_elements[i].value < pivot->value)
        {
            iS++;
            std::swap(m_elements[iS], m_elements[i]);
        }
    }
    for (long i = (long)start; i <= end - 1 && m_state != State::BeingCollected; i++)
    {
        m_elements[i].color = sf::Color::White;
    }
    std::swap(m_elements[iS + 1], m_elements[end]);
    return (iS + 1);
}

void QuickSort::Quick(long start, long end)
{
    if (start < end && m_state != State::BeingCollected)
    {
        PauseCheck();
        long iP = Partition(start, end); //partion index

        Quick(start, iP - 1);
        Quick(iP + 1, end);
    }
}