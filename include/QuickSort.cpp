#include "QuickSort.hpp"

#include "Math.hpp"

QuickSort::QuickSort(std::string const &sortName)
    : Sort(sortName)
{
}

void QuickSort::Step()
{
    Quick(0, m_allBars->size() - 1);
}

// BarIter QuickSort::Partition(BarIter start, BarIter end)
// {
//     BarContainer::Bar pivot = **end;
//     BarIter iS = start - 1;

//     for (int i = 0; start + i <= end - 1 && !m_isBeingCollected; i++)
//     {
//         std::this_thread::sleep_for(std::chrono::microseconds(m_stepTime));
//         if (((*m_allBars)[i]->GetValue() < pivot.GetValue()))
//         {
//             iS++;
//             std::swap(**iS, *(*m_allBars)[i]);
//         }
//     }
//     std::swap(**(iS + 1), **end);
//     return (iS + 1);
// }

// void QuickSort::Quick(BarIter start, BarIter end)
// {
//     if (start < end && !m_isBeingCollected)
//     {
//         BarIter iP = Partition(start, end); //partion index

//         Quick(start, iP - 1);
//         Quick(iP + 1, end);
//     }
// }

int QuickSort::Partition(int start, int end)
{
    BarContainer::Bar pivot = *(*m_allBars)[end];
    int iS = start - 1;

    for (int i = start; i <= end - 1 && !m_isBeingCollected; i++)
    {
        (*m_allBars)[i]->SetColor(sf::Color::Blue);
    }
    for (int i = start; i <= end - 1 && !m_isBeingCollected; i++)
    {
        ContinueIfNotPaused();
        (*m_allBars)[i]->SetColor(sf::Color::Red);
        std::this_thread::sleep_for(std::chrono::microseconds(m_stepTime));
        if (((*m_allBars)[i]->GetValue() < pivot.GetValue()))
        {
            iS++;
            std::swap((*m_allBars)[iS], (*m_allBars)[i]);
        }
    }
    for (int i = start; i <= end - 1 && !m_isBeingCollected; i++)
    {
        (*m_allBars)[i]->SetColor(sf::Color::White);
    }
    std::swap((*m_allBars)[iS + 1], (*m_allBars)[end]);
    return (iS + 1);
}

void QuickSort::Quick(int start, int end)
{
    if (start < end && !m_isBeingCollected)
    {
        ContinueIfNotPaused();
        int iP = Partition(start, end); //partion index

        Quick(start, iP - 1);
        Quick(iP + 1, end);
    }
}