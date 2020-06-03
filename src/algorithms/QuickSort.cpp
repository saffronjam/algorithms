#include "QuickSort.h"

void QuickSort::Sort()
{
    Quick(0, m_bars.size() - 1);
}

long QuickSort::Partition(long start, long end)
{
    Bar *pivot = &m_bars[end];
    long iS = start - 1;

    for (long i = (long)start; i <= end - 1 && m_state != State::BeingCollected; i++)
    {
        m_bars[i].color = sf::Color::Blue;
    }
    for (long i = (long)start; i <= end - 1 && m_state != State::BeingCollected; i++)
    {
        PauseCheck();
        m_bars[i].color = sf::Color::Red;
        SleepDelay();
        if (m_bars[i].value < pivot->value)
        {
            iS++;
            std::swap(m_bars[iS], m_bars[i]);
        }
    }
    for (long i = (long)start; i <= end - 1 && m_state != State::BeingCollected; i++)
    {
        m_bars[i].color = sf::Color::White;
    }
    std::swap(m_bars[iS + 1], m_bars[end]);
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